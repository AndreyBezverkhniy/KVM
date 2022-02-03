#pragma once
#include "operand.h"

class SimpleExpression : public Operand{
public:
	SimpleExpression();
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
