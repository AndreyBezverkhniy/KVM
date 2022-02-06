#include "function_signature.h"
#include "utils.h"

FunctionSignature::FunctionSignature(){}
FunctionSignature::FunctionSignature(string func_name,int arg_n){
	this->func_name=func_name;
	this->arguments.resize(arg_n);
}
bool FunctionSignature::Save(ostream &os) const {
	if(!USave(os,func_name)){
		return false;
	}
	int size=arguments.size();
	if(!USave(os,size)){
		return false;
	}
	for(int i=0;i<size;i++){
		if(!USave(os,arguments[i])){
			return false;
		}
	}
	return true;
}
bool FunctionSignature::Load(istream &is){
	if(!ULoad(is,func_name)){
		return false;
	}
	int size;
	if(!ULoad(is,size)){
		return false;
	}
	arguments.resize(size);
	for(int i=0;i<size;i++){
		if(!ULoad(is,arguments[i])){
			return false;
		}
	}
	return true;
}
bool operator<(FunctionSignature l,FunctionSignature r){
	if(l==r){ // for case of several main declarations
		return false;
	}
	return l.arguments.size()<r.arguments.size() ||
	l.arguments.size()==r.arguments.size() && l.func_name<r.func_name;
}
bool operator==(FunctionSignature l,FunctionSignature r){
	if(l.func_name!=r.func_name){
		return false;
	}
	return l.func_name=="main" || // for case of several main declarations
	l.arguments.size()==r.arguments.size();
}
