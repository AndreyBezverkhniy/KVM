#include "utils.h"

bool USave(ostream &os,bool b){
	os.put(b?'T':'F');
	os.put('.');
	return os.good();
}
bool ULoad(istream &is,bool &b){
	char ch;
	is.get(ch);
	b=(ch=='T');
	char dot;
	is.get(dot);
	return (ch=='T' || ch=='F') && dot=='.' && is.good();
}
bool USave(ostream &os,char ch){
	os.put(ch);
	return os.good();
}
bool ULoad(istream &is,char &ch){
	is.get(ch);
	return is.good();
}
bool USave(ostream &os,int n){
	os<<n<<".";
	return os.good();
}
bool ULoad(istream &is,int &n){
	is>>n;
	char ch;
	is>>ch;
	return ch=='.' && is.good();
}
bool USave(ostream &os,string str){
	USave(os,(int)str.size());
	os<<str;
	USave(os,'.');
	return os.good();
}
bool ULoad(istream &is,string &str){
	int len;
	ULoad(is,len);
	char *arr=new char[len];
	is.read(arr,len);
	str=string(arr,len);
	delete[] arr;
	char ch;
	ULoad(is,ch);
	return ch=='.' && is.good();
}

bool IsIdentifier(string str){
	if(str.size()==0 || !isalpha(str[0])){
		return false;
	}
	for(auto ch:str){
		if(!isalpha(ch)&&!isdigit(ch)){
			return false;
		}
	}
	return true;
}
bool IsNumber(string str){
	if(str.size()==0 || str.size()>1 && str[0]=='0'){
		return false;
	}
	for(auto ch:str){
		if(!isdigit(ch)){
			return false;
		}
	}
	return true;
}
int ToInt(string str){
	int value=0;
	for(char ch:str){
		value*=10;
		value+=ch-'0';
	}
	return value;
}
