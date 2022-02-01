#include "var_declaration.h"
#include "utils.h"

Var::Var(){}
bool Var::SaveInner(ostream &os) const {
	int size=(int)declarations.size();
	if(!USave(os,size)){
		return false;
	}
    for(auto pair:declarations){
        if(!USave(os,pair.first)){
            return false;
        }
        if(!USave(os,pair.second)){
            return false;
        }
    }
	return true;
}
bool Var::LoadInner(istream &is){
	int size;
	if(!ULoad(is,size)){
		return false;
	}
    string variable;
    int value;
    for(int i=0;i<size;i++){
        if(!ULoad(is,variable)){
            return false;
        }
        if(!ULoad(is,value)){
            return false;
        }
        declarations.push_back(pair<string,int>(variable,value));
    }
	return true;
}
