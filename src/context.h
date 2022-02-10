#pragma once
#include <string>
#include <iostream>
#include <map>
#include <memory>

using namespace std;

class Context{
public:
	shared_ptr<Context> parent;
	map<string,int> variables;
	bool Have(string key) const;
	void SetKeyValue(string key,int value);
	int GetValue(string key);
	bool DeleteKey(string key);
	void SetParentContext(shared_ptr<Context> parent);
	shared_ptr<Context> GetParentContext();
	bool Load(istream &is);
	bool Save(ostream &os) const;
};
