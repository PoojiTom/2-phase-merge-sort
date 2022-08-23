#include<bits/stdc++.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include<fstream>
#include<ctime>
#include<ratio>
#include<chrono>
using namespace std;
#define int long long 
map<string,int>mp;
vector<int>order,pref,siz,temp_rows;
int mm;
int asc=-1,row_size,num_rows,file_size,input_size,input_rows,num_files;
#undef int
bool comp(vector<string>&a, vector<string>&b){
	for(auto x : order){
		if(a[x]==b[x]){
			continue;
		}
		else{
			if(asc==1){
				return a[x]<b[x];
			}
			else 
				return a[x]>b[x];
		}
	}
	return 0;
}
bool comp2(const pair<vector<string>,int>&a,const pair<vector<string>,int>&b){
	for(auto x : order){
		if(a.first[x]==b.first[x]){
			continue;
		}
		else{
			if(asc==1){
				return a.first[x]>b.first[x];
			}
			else
				return a.first[x]<b.first[x];
		}
	}
	return a.second>b.second;
}
int main(int argc, char* argv[]){
#define int long long
	 using namespace std::chrono;
	         high_resolution_clock::time_point t1 = high_resolution_clock::now();
	fstream file;
	string x;
	for (int i=0;i<argc ;i++){
		cout << i << ' ' << argv[i] << '\n';
	}
	file.open("metadata.txt",ios::in);
	if(!file.good()){
		cout << "metadata not found\n";
		exit(-1);
	}
	int l=0;
	int flag=system("rm -r temps");
	//rmdir("temps");
	int check=mkdir("temps", 0777);
	if(check){
		cout << "error in creating a folder\n";
		exit(-1);
	}
	if(argc<6){
		cout << "no enough arguments\n";
		exit(-1);
	}
	while(getline(file,x)){
		for(int i=0;i<x.length();i++){
			if(x[i]==','){
				mp[x.substr(0,i)]=l;
				siz.push_back(stoi(x.substr(i+1,x.length())));
				if(l){
					pref.push_back(pref[l-1]+siz[l]+2);
				}
				else pref.push_back(siz[l]);
				//	cout << pref[l] << ' ';
			}
		}
		if(siz.size()<l+1){
			cout << siz.size() << ' ';
			cout << "some error in metadata could not find comma\n";
		}
		l+=1;
	}
	//cout << '\n';
	file.close();
	string input=argv[1],output=argv[2];
	mm=((int)min(stoi(argv[3]),5))*(int)pow(10,6);
	//cout << argv[4];
	//	char  as[]=['a','s','c'];
	string ar=argv[4];
	if(ar=="asc"){
		asc=1;
		//	cout << "hi\n";
	}
	row_size=pref[l-1]+2;
	for(int i=5;i<argc;i++){
		if(mp.find(argv[i])==mp.end()){
			cout << "column not found\n";
			exit(-1);
		}
		order.push_back(mp[argv[i]]);
	}
	num_rows=(mm/row_size);
	file_size=num_rows*row_size;
	file.open(input,ios::in);
	if(!file.good()){
		cout << "cant read input\n";
	}
	file.seekg(0,file.end);
	input_size=file.tellg();
	cout << "file is of " << input_size << " bytes\n";
	if(input_size%row_size){
		cout << "file size is not multiple of row size \n";
		exit(-1);
	}
//	cout << "records in main mem and row(record) size:" << num_rows << ' '<< row_size <<'\n';
	file.close();
	input_rows=input_size/row_size;
	cout << "rows in file input--:"<< input_rows << '\n';
//	cout << "input rows and number of rows in created files :" << input_rows << ',' << num_rows << '\n';
	num_files=(input_rows/num_rows);
	if(input_rows%num_rows){
		num_files+=1;
	}
	if(num_files+1>num_rows){
		cout << "2-phase not possible\n";
		exit(-1);
	}
//	cout << "files = " << num_files << '\n';
	cout << "attempting to write into files in phase1 \n";
	ifstream inpu(input);
	//	input_rows-((num_files-1)*num_rows)
	for(int i=0;i<num_files;i++){
		int n;
		n=min(num_rows,input_rows-(i*(num_rows)));
		char *me=new char[n*row_size+1];
		inpu.read(me,n*row_size);
		me[n*row_size]='\0';
		//cout << me[n*row_size-1] << '\n';
		stringstream mee(me);
		string X;
		/*while(getline(mee,x)){
		  cout << x << '\n';
		  }*/
		vector<vector<string>>lilly;
		ofstream fin("./temps/file"+to_string(i+1)+".txt");
		if(!fin.good()){
			cout << "cant open files \n";
			exit(-1);
		}
		for(int j=0;j<n;j++){
			getline(mee,X);
			vector<string>temp;
			//int tej=0;
			for(int k=0;k<siz.size();k++){
				string x=X,he;
				//cout  << tej <<' '<< tej+siz[k] << '\n';
				//cout << x.substr(tej,tej+siz[k]) << '\n';
				if(k==0){
					he=x.substr(0,siz[k]);
				}
				else
					he=x.substr(pref[k-1]+2,siz[k]);
				temp.push_back(he);
				//	tej+=(siz[k]+2);
			}
			lilly.push_back(temp);
			temp.clear();
		}
		sort(lilly.begin(),lilly.end(),comp);
		vector<string>lilly1;
		for(int j=0;j<lilly.size();j++){
			string lilly2=lilly[j][0];
			for(int k=1;k<lilly[0].size();k++){
				lilly2=lilly2+"  "+lilly[j][k];
			}
			lilly1.push_back(lilly2);
		}
		stringstream finale;
		copy(lilly1.begin(),lilly1.end(),ostream_iterator<string>(finale, "\r\n"));
		fin<<finale.rdbuf();
		fin.flush();
		fin.close();
		lilly.clear();
		lilly1.clear();
	//	cout << "sorted and written in file" << i+1 << "\n";
		//cout << '\n';
	}
	ifstream temp[num_files];
	int temp_rows=num_rows/(num_files+1);
	vector<vector<string>>vec[num_files];
	int temp_sizes[num_files];
	vector<pair<vector<string>,int>>Heap;
	cout << "phase2 started :\n";
	for(int i=0;i<num_files;i++){
		temp[i].open("./temps/file"+to_string(i+1)+".txt");
		if(!temp[i].good()){
			cout << i << ' ' ;
			exit(-1);
		}
		temp[i].seekg(0,temp[i].end);
		temp_sizes[i]=temp[i].tellg()/row_size;
		temp[i].close();
		temp[i].open("./temps/file"+to_string(i+1)+".txt");
		int n=min(temp_rows,temp_sizes[i]);
		if(n==0)
			continue;
		temp_sizes[i]-=n;
		char *buff=new char[n*row_size+1];
		buff[n*row_size]='\0';
		temp[i].read(buff,n*row_size);
		stringstream mee(buff);
		string X;
		while(getline(mee,X)){
			//getline(mee,X);
			vector<string>temp;
			//int tej=0;
			for(int k=0;k<siz.size();k++){
				string x=X,he;
				//cout  << tej <<' '<< tej+siz[k] << '\n';
				//cout << x.substr(tej,tej+siz[k]) << '\n';
				if(k==0){
					he=x.substr(0,siz[k]);
				}
				else
					he=x.substr(pref[k-1]+2,siz[k]);
				temp.push_back(he);
				//      tej+=(siz[k]+2);
			}
			vec[i].push_back(temp);
			temp.clear();
		}
		Heap.push_back({vec[i][0],i});
		vec[i].erase(vec[i].begin());
		push_heap(Heap.begin(),Heap.end(),comp2);
	}
	ofstream out(output);
	vector<string>lil;
	int mohit=0;
	while(Heap.size()){
		//cout << "hi\n";
		//if((mohit%num_rows)==0){
		//	cout << "finished writing " << mohit/num_rows <<" mm\n";
		//}
		mohit+=1;
		string lil1=Heap[0].first[0];
		for(int i=1;i<Heap[0].first.size();i++){
			lil1=lil1+"  "+Heap[0].first[i];
		}
		int index=Heap[0].second;
		lil.push_back(lil1);
		if(lil.size()==temp_rows){
			stringstream finale;
			copy(lil.begin(),lil.end(),ostream_iterator<string>(finale, "\n"));
			lil.clear();
			out<<finale.rdbuf();
			finale.flush();
		}
		pop_heap(Heap.begin(),Heap.end(),comp2);
		Heap.pop_back();
		if(vec[index].size()==0){
			int n=min(temp_rows,temp_sizes[index]);
			if(n==0)
				continue;
			temp_sizes[index]-=n;
			char *buff=new char[n*row_size+1];
			buff[n*row_size]='\0';
			temp[index].read(buff,n*row_size);
			stringstream mee(buff);
			string X;
			for(int j=0;j<n;j++){
				getline(mee,X);
				vector<string>temp;
				//int tej=0;
				for(int k=0;k<siz.size();k++){
					string x=X,he;
					//cout  << tej <<' '<< tej+siz[k] << '\n';
					//cout << x.substr(tej,tej+siz[k]) << '\n';
					if(k==0){
						he=x.substr(0,siz[k]);
					}
					else
						he=x.substr(pref[k-1]+2,siz[k]);
					temp.push_back(he);
					//      tej+=(siz[k]+2);
				}
				vec[index].push_back(temp);
				temp.clear();
			}
		}
		if(vec[index].size()){
			Heap.push_back({vec[index][0],index});
			push_heap(Heap.begin(),Heap.end(),comp2);
			vec[index].erase(vec[index].begin());
		}
	}
	stringstream finale;
	copy(lil.begin(),lil.end(),ostream_iterator<string>(finale, "\n"));
	lil.clear();
	out<<finale.rdbuf();
	finale.flush();
	cout << "finished :))\n";
	        high_resolution_clock::time_point t2 = high_resolution_clock::now();
		        duration<double, std::milli> time_span1 = t2 - t1;
			cout << " took this time : " << time_span1.count() << '\n';
	int flag1=system("rm -r temps");
	//	cout << asc << '\n';0
	//cout << row_size << ' ' << num_rows << ' ' << file_size << '\n';
}	
