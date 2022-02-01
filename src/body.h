#include <iostream>
#include <vector>
#include "instruction.h"

using namespace std;

class Body{
public:
	vector<Instruction> instructions;
	Body();
	bool Save(ostream &os) const;
	bool Load(istream &is);
};
