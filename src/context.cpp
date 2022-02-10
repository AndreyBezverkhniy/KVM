#include "context.h"
#include "utils.h"

bool Context::Have(string key) const {
    return variables.find(key)!=variables.end();
}
void Context::SetKeyValue(string key,int value){
    variables[key]=value;
}
int Context::GetValue(string key){
    return variables[key];
}
bool Context::DeleteKey(string key){
    return variables.erase(key)==0;
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
        SetKeyValue(key,value);
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
