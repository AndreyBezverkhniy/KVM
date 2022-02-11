#include "context.h"
#include "utils.h"

Context* Context::GetContextByVariableName(string key){
    if(variables.find(key)!=variables.end()){
        return this;
    }
    if(parent){
        return parent->GetContextByVariableName(key);
    }
    return nullptr;
}
bool Context::Have(string key){
    return GetContextByVariableName(key)!=nullptr;
}
void Context::SetValueInContext(string key,int value){
    variables[key]=value;
}
bool Context::SetValueInChain(string key,int value){
    if(auto ptr=GetContextByVariableName(key)){
		ptr->variables[key]=value;
		return true;
	}
	return false;
}
int Context::GetValueInChain(string key){
    return GetContextByVariableName(key)->variables[key];
}
void Context::SetParentContext(shared_ptr<Context> parent){
    this->parent=parent;
}
shared_ptr<Context> Context::GetParentContext(){
    return parent;
}
bool Context::Load(istream &is){
    int size;
    if(!ULoad(is,size)){
        return false;
    }
    string key;
    int value;
    for(int i=0;i<size;i++){
        if(!ULoad(is,key)){
            return false;
        }
        if(!ULoad(is,value)){
            return false;
        }
        SetValueInContext(key,value);
    }
    return true;
}
bool Context::Save(ostream &os) const {
    if(!USave(os,(int)variables.size())){
        return false;
    }
    for(auto pair:variables){
        if(!USave(os,pair.first)){
            return false;
        }
        if(!USave(os,pair.second)){
            return false;
        }
    }
    return true;
}
void PrintContextChain(shared_ptr<Context> ptr){
	if(!ptr){
		cout<<"Null Context Pointer"<<endl;
		return;
	}
	PrintContextChain(ptr->parent);
	cout<<"{ ";
	for(auto e:ptr->variables){
		cout<<e.first<<"="<<e.second<<" ";
	}
	cout<<"}"<<endl;
}
