#pragma once
#include "operand.h"

#define NUMBER_TYPE string("number")
#define VARIABLE_NAME_TYPE string("varname")

class SimpleExpression : public Operand{
public:
    string GetSimpleExpressionType() const;
	bool Save(ostream &os) const;
	virtual bool SaveInner(ostream &os) const;
	static bool Load(istream &is,shared_ptr<SimpleExpression> &simple_expression_ptr);
};
