#pragma once
#include "simple_expression.h"

class Number : public SimpleExpression{
public:
    int n;
	Number();
	Number(int n);
    void SetValue(int n);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
