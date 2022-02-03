#pragma once
#include "simple_expression.h"

class VariableName : public SimpleExpression{
public:
    string name;
	VariableName();
    void SetName(string name);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
