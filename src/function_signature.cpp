#include "function_signature.h"
#include "utils.h"

FunctionSignature::FunctionSignature(){}
FunctionSignature::FunctionSignature(string func_name,int arg_n){
	this->func_name=func_name;
	this->arg_n=arg_n;
}
bool FunctionSignature::Save(ostream &os) const {
	if(!USave(os,func_name)){
		return false;
	}
	if(!USave(os,arg_n)){
		return false;
	}
	return true;
}
bool FunctionSignature::Load(istream &is){
	if(!ULoad(is,func_name)){
		return false;
	}
	if(!ULoad(is,arg_n)){
		return false;
	}
	return true;
}
bool operator<(FunctionSignature l,FunctionSignature r){
	if(l==r){ // for case of several main declarations
		return false;
	}
	return l.arg_n<r.arg_n ||
	l.arg_n==r.arg_n && l.func_name<r.func_name;
}
bool operator==(FunctionSignature l,FunctionSignature r){
	if(l.func_name!=r.func_name){
		return false;
	}
	return l.func_name=="main" || // for case of several main declarations
	l.arg_n==r.arg_n;
}
