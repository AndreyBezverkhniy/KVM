#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include "instruction.h"

using namespace std;

class Block : public Instruction{
public:
	vector<shared_ptr<Instruction>> instructions;
	Block();
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
