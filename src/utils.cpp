#include "utils.h"

bool USave(ostream &os,string str){
	USave(os,str.size());
	os<<str<<".";
}
bool ULoad(istream &is,string str);
bool USave(ostream &os,int n){
	os<<n<<".";
}
bool ULoad(istream &is,int n);
