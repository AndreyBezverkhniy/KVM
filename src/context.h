#include <string>
#include <iostream>
#include <map>

using namespace std;

class Context{
	map<string,int> variables;
public:
	void SetKeyValue(string key,int value);
	bool DeleteKey(string key);
	bool Load(istream &is);
	bool Save(ostream &os) const;
};
