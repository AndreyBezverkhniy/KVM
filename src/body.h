#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "instruction.h"

using namespace std;

class Body{
public:
	vector<shared_ptr<Instruction>> instructions;
	Body();
	bool Save(ostream &os) const;
	bool Load(istream &is);
};
