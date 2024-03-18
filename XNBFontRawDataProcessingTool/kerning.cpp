#include <iostream>
#include <string>
#include <sstream>
using namespace std;

inline int convert(string str){
	int ret;
	stringstream ss;
	ss << str;
	ss >> ret;
	return ret;
}

int main(){
	freopen("uni 05_53 12px.yml", "r", stdin);
	freopen("uni 05_53 12px.yml_sample.yml", "w", stdout);
	string str;
	getline(cin, str);
	while(str.find("glyphs") == string::npos){
		cout << str << endl;
		getline(cin, str);
	}
	cout << str << endl;
	getline(cin, str);
	while(str.find("-  #!Rectangle") != string::npos){
		cout << str << endl;
		getline(cin, str);
		cout << str.substr(0, 15) << convert(str.substr(15)) - 1 << endl;
		getline(cin, str);
		cout << str.substr(0, 15) << convert(str.substr(15)) - 1 + 131 << endl;
		getline(cin, str);
		cout << str.substr(0, 19) << convert(str.substr(19)) + 2 << endl;
		getline(cin, str);
		cout << str.substr(0, 20) << convert(str.substr(20)) + 2 << endl;
		getline(cin, str);
		cout << str << endl;
		getline(cin, str);
	}
	while(str.find("cropping") == string::npos){
		cout << str << endl;
		getline(cin, str);
	}
	cout << str << endl;
	getline(cin, str);
	while(str.find("-  #!Rectangle") != string::npos){
		cout << str << endl;
		getline(cin, str);
		cout << str.substr(0, 15) << convert(str.substr(15)) + 3 << endl;
		getline(cin, str);
		cout << str.substr(0, 15) << convert(str.substr(15)) + 6 << endl;
		getline(cin, str);
		cout << str << endl;
		getline(cin, str);
		cout << str << endl;
		getline(cin, str);
		cout << str << endl;
		getline(cin, str);
	}
	while(str.find("kerning") == string::npos){
		cout << str << endl;
		getline(cin, str);
	}
	cout << str << endl;
	getline(cin, str);
	while(str.find("-  #!Vector3") != string::npos){
		cout << str << endl;
		getline(cin, str);
		cout << str << endl;
		getline(cin, str);
		cout << str.substr(0, 15) << convert(str.substr(15)) + 6 << endl;
		getline(cin, str);
		cout << str << endl;
		getline(cin, str);
		cout << str << endl;
		getline(cin, str);
	}
	do cout << str << endl; while(getline(cin, str));
	return 0;
} 
