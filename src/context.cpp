#include "context.h"
#include "utils.h"

void Context::SetKeyValue(string key,int value){
    variables[key]=value;
}
bool Context::DeleteKey(string key){
    return variables.erase(key)==0;
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