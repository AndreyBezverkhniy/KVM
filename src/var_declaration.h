#pragma once
#include "instruction.h"
#include "expression.h"
#include <vector>

class Var : public Instruction{
public:
	vector<pair<string,shared_ptr<Expression>>> declarations;
	Var();
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
