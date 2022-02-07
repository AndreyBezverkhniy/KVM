#include "body.h"
#include "utils.h"

Body::Body(){}
bool Body::Save(ostream &os) const {
    int size=argument_names.size();
	if(!USave(os,size)){
		return false;
	}
    for(auto name:argument_names){
        if(!USave(os,name)){
            return false;
        }
    }
	if(!block->SaveInner(os)){
		return false;
	}
	return true;
}
bool Body::Load(istream &is){
    int size;
	if(!ULoad(is,size)){
		return false;
	}
    string name;
    for(int i=0;i<size;i++){
        if(!ULoad(is,name)){
            return false;
        }
        argument_names.push_back(name);
    }
    block=make_shared<Block>();
	if(!block->LoadInner(is)){
		return false;
	}
	return true;
}
