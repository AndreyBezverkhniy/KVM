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
	if(!USave(os,(int)arguments.size())){
		return false;
	}
	return true;
}
bool FunctionSignature::Load(istream &is){
	if(!ULoad(is,func_name)){
		return false;
	}
	int arg_n;
	if(!ULoad(is,arg_n)){
		return false;
	}
	arguments.resize(arg_n);
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
