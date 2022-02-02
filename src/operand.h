#pragma once
#include "expression.h"

class Operand : public Expression{
public:
	Operand();
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
