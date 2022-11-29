#include<iostream>
#include<vector>
#include<string.h>
#include<fstream>
using namespace std;
vector<string> result;

int compare_id(string s, string s1){
	int i = 0;
	string id = "";
	int x = s.length();
	while(s[i] != ',' && i < x){
		id += s[i];
		i++;
	}
	string id1 = "";
	i = 0;
	x = s1.length();
	while(s1[i] != ',' && i < x){
		id1 += s1[i];
		i++;
	}
	return id.compare(id1);
}

int partition(vector<string> &v, int l, int r){
	int i = l - 1, j = r + 1;
	string pivot = v[rand()%(r - l + 1) + l];
	while(true){
		do{
			i++;
		}while(compare_id(v[i], pivot) == -1);
		do{
			j--;
		}while(compare_id(v[j], pivot) == 1);
		if(i < j) swap(v[i], v[j]);
		else return j;
	}
}

void quick_sort(vector<string> &v, int l, int r){
	if(l >= r) return;
	int pivot = partition(v, l, r);
	quick_sort(v, l, pivot);
	quick_sort(v, pivot + 1, r);
}

void read_data_from_file(vector<string> &result, ifstream& file_in){
	string x;
	while(!file_in.eof()){
		getline(file_in, x);
		result.push_back(x);
	}
	file_in.close();
}

void write_data_from_vector_into_file(vector<string> &result, ofstream &file_out){
	for(auto it : result){
		file_out << it << endl;
	}
	result.clear();
	file_out.close();
}

void merge_3file(ifstream &file_1, ifstream &file_2, ifstream &file_3, ofstream &fout){
	string x; getline(file_1, x);
	string y; getline(file_2, y);
	string z; getline(file_3, z);
	while(!file_1.eof() && !file_2.eof() && !file_3.eof()){
		if(compare_id(x, y) != 1 && compare_id(x, z)){
			fout << x << endl;
			getline(file_1, x);
			continue;
		}
		if(compare_id(y, x) != 1 && compare_id(y, z) != 1){
			fout << y << endl;
			getline(file_2, y);
			continue;
		}
		if(compare_id(z, x) != 1 && compare_id(z, y) != 1){
			fout << z << endl;
			getline(file_3, z);
			continue;
		}
	}
	if(file_1.eof()){
		while(!file_2.eof() && !file_3.eof()){
			if(compare_id(y, z) != 1){
				fout << y << endl;
				getline(file_2, y);
			}
			else{
				fout << z << endl;
				getline(file_3, z);
			}
		}
		while(!file_2.eof()){
			fout << y << endl;
			getline(file_2, y);
		}
		while(!file_3.eof()){
			fout << z << endl;
			getline(file_3, z);
		}
	}
	else if(!file_2.eof()){
		while(!file_1.eof() && !file_3.eof()){
			if(compare_id(x, z)  != 1){
				fout << x << endl;
				getline(file_1, x);
			}
			else{
				fout << z << endl;
				getline(file_3, z);
			}
		}
		while(!file_1.eof()){
			fout << x << endl;
			getline(file_1, x);
		}
		while(!file_3.eof()){
			fout << z << endl;
			getline(file_3, z);
		}
	}
	else{
		while(!file_1.eof() && !file_2.eof()){
			if(compare_id(x, y) != 1){
				fout << x << endl;
				getline(file_1, x);
			}
			else{
				fout << y << endl;
				getline(file_3, y);
			}
		}
		while(!file_1.eof()){
			fout << x << endl;
			getline(file_1, x);
		}
		while(!file_2.eof()){
			fout << y << endl;
			getline(file_2, y);
		}
	}
	file_1.close(); file_2.close(); file_3.close();
	fout.close();
}

void merge_2file(ifstream &file_1, ifstream &file_2, ofstream &fout){
	string x; getline(file_1, x);
	string y; getline(file_2, y);
	while(!file_1.eof() && !file_2.eof()){
		if(compare_id(x, y) != 1){
			fout << x << endl;
			getline(file_1, x);
		}
		else{
			fout << y << endl;
			getline(file_2, y);
		}
	}
	while(!file_1.eof()){
		fout << x << endl;
		getline(file_1, x);
	}
	while(!file_2.eof()){
		fout << y << endl;
		getline(file_2, y);
	}
	file_1.close(); file_2.close();
	fout.close();
}

int main(){
	//tiet kiem bo nho
	//remove("Books_rating.csv");
	//after split Book_rating.csv into 10 smaller file
	ifstream fin_1, fin_2, fin_3;
	ofstream fout_1;
	int length_vector;
	string title = "Id,Title,Price,User_id,profileName,review/helpfulness,review/score,review/time,review/summary,review/text";
	//sort each file
	for(int i = 1; i <= 10; i++){
		string x = "file_split_" + to_string(i) + ".csv";
		fin_1.open(x, ios::in);
		read_data_from_file(result, fin_1);
		length_vector = result.size() - 1;
		quick_sort(result, 0, length_vector);
		fout_1.open(x, ios::out);
		write_data_from_vector_into_file(result, fout_1);
	}

	//merge file_split_ 1, 2, 3 into file_result_123.csv after that delete these three files
	fin_1.open("file_split_1.csv", ios::in);
	fin_2.open("file_split_2.csv", ios::in);
	fin_3.open("file_split_3.csv", ios::in);
	fout_1.open("file_result_123.csv", ios::out);
	merge_3file(fin_1, fin_2, fin_3, fout_1);
	remove("file_split_1.csv");
	remove("file_split_2.csv");
	remove("file_split_3.csv");

	//merge file_split_ 4, 5, 6 into file_result_456.csv after that delete these three files
	fin_1.open("file_split_4.csv", ios::in);
	fin_2.open("file_split_5.csv", ios::in);
	fin_3.open("file_split_6.csv", ios::in);
	fout_1.open("file_result_456.csv", ios::out);
	merge_3file(fin_1, fin_2, fin_3, fout_1);
	remove("file_split_4.csv");
	remove("file_split_5.csv");
	remove("file_split_6.csv");

	//merge file_split_ 7, 8, 9 into file_result_789.csv after that delete these three files
	fin_1.open("file_split_7.csv", ios::in);
	fin_2.open("file_split_8.csv", ios::in);
	fin_3.open("file_split_9.csv", ios::in);
	fout_1.open("file_result_789.csv", ios::out);
	merge_3file(fin_1, fin_2, fin_3, fout_1);
	remove("file_split_7.csv");
	remove("file_split_8.csv");
	remove("file_split_9.csv");

	//merge file file_result_123, 456, 789 into file_result_all.csv after that delete these three files
	fin_1.open("file_result_123.csv", ios::in);
	fin_2.open("file_result_456.csv", ios::in);
	fin_3.open("file_result_789.csv", ios::in);
	fout_1.open("file_result_all.csv", ios::out);
	merge_3file(fin_1, fin_2, fin_3, fout_1);
	remove("file_result_123.csv");
	remove("file_result_456.csv");
	remove("file_result_789.csv");

	//merge file_split_10.csv and file_result_all into sorted_books_rating.csv after that remove these two files
	fin_1.open("file_result_all.csv", ios:: in);
	fin_2.open("file_split_10.csv", ios::in);
	fout_1.open("sorted_books_rating.csv", ios::out);
	fout_1 << title << endl;
	merge_2file(fin_1, fin_2, fout_1);
	remove("file_result_all.csv");
	remove("file_split_10.csv");
}

