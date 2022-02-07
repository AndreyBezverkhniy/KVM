#pragma once
#include <string>
#include <iostream>
#include <vector>

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
	friend bool operator==(const FunctionSignature l,const FunctionSignature r);
};
