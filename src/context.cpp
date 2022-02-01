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
    ULoad(is,size);
    string key;
    int value;
    for(int i=0;i<size;i++){
        ULoad(is,key);
        ULoad(is,value);
        SetKeyValue(key,value);
    }
}
bool Context::Save(ostream &os){
    USave(os,(int)variables.size());
    for(auto pair:variables){
        USave(os,pair.first);
        USave(os,pair.second);
    }
}
