#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

class Context;

class Program{
public:
	Context global;
	vector<Context> contextStack;
	map<FunctionSignature,Body> functions;
	int Calc();
	bool Save(ostream &os);
	void Load(istream &is);
};

class Instruction{
public:
	int Calc();
	bool Save(ostream &os);
	void LoadInstruction(istream &is){}
private:
	void Load(istream &is);
};
