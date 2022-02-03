#include "number.h"
#include "utils.h"

Number::Number(){}
void Number::SetValue(int n){
	this->n=n;
}
bool Number::SaveInner(ostream &os) const {
	if(!USave(os,n)){
		return false;
	}
return true;
}
bool Number::LoadInner(istream &is){
	if(!ULoad(is,n)){
		return false;
	}
	return true;
}
