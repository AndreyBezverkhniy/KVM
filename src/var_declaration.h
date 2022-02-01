#pragma once
#include "instruction.h"
#include <vector>

class Var : public Instruction{
public:
	vector<pair<string,int>> declarations;
	Var();
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
