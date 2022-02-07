#pragma once
#include <string>
#include <map>
#include "context.h"
#include "function_signature.h"
#include "body.h"

using namespace std;

class Program{
public:
	Context global;
	map<FunctionSignature,Body> functions;
	Program();
	bool Save(string path) const;
	bool Save(ostream &os) const;
	bool Load(string path);
	bool Load(istream &is);
};
