#include <string>
#include <iostream>

#pragma once
using namespace std;

class FunctionSignature{
public:
	string func_name;
	int arg_n;
	FunctionSignature();
	FunctionSignature(string func_name,int arg_n);
	bool Save(ostream &os) const;
	bool Load(istream &is);
	friend bool operator<(const FunctionSignature l,const FunctionSignature r);
};
