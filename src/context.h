#pragma once
#include <string>
#include <iostream>
#include <map>
#include <memory>

using namespace std;

class Context{
private:
	shared_ptr<Context> parent;
	map<string,int> variables;
	Context* GetContextByVariableName(string name);
public:
	bool Have(string key);
	void SetKeyValue(string key,int value);
	bool SetValueInChain(string key,int value);
	int GetValueInChain(string key);
	void SetParentContext(shared_ptr<Context> parent);
	shared_ptr<Context> GetParentContext();
	bool Load(istream &is);
	bool Save(ostream &os) const;
	friend void PrintContextChain(shared_ptr<Context> ptr);
};
