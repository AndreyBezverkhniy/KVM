#include "context.h"
#include "utils.h"

void Context::SetKeyValue(string key,int value){
    variables[key]=value;
}
bool Context::DeleteKey(string key){
    return variables.erase(key)==0;
}
bool Context::Load(istream &is){}
bool Context::Save(ostream &os){
    USave(os,variables.size());
    for(auto pair:variables){
        USave(os,pair.first);
        USave(os,pair.second);
    }
}
