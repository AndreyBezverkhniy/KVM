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
        if(!pair.second->Save(os)){
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
    shared_ptr<Expression> value=make_shared<Expression>();
    for(int i=0;i<size;i++){
        if(!ULoad(is,variable)){
            return false;
        }
        if(!Expression::Load(is,value)){
            return false;
        }
        declarations.push_back(pair<string,shared_ptr<Expression>>(variable,value));
    }
	return true;
}
