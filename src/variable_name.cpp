#include "variable_name.h"
#include "utils.h"

VariableName::VariableName(){}
VariableName::VariableName(string name){
	SetName(name);
}
void VariableName::SetName(string name){
	this->name=name;
}
bool VariableName::SaveInner(ostream &os) const {
	if(!USave(os,name)){
		return false;
	}
    return true;
}
bool VariableName::LoadInner(istream &is){
	if(!ULoad(is,name)){
		return false;
	}
	return true;
}
