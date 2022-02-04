#pragma once
#include "operand.h"

#define NUMBER_TYPE "number"
#define VARIABLE_NAME_TYPE "varname"
#define FUNCTION_CALL_TYPE "fcall"

class SimpleExpression : public Operand{
public:
    string GetType() const override;
	static bool IsSimpleExpression(string type);
	static bool Load(istream &is,shared_ptr<SimpleExpression> &simple_expression_ptr);
	static bool LoadTyped(string type,istream &is,shared_ptr<SimpleExpression> &simple_expression_ptr);
};
