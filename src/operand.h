#pragma once
#include "expression.h"

#define SIMPLE_EXPRESSION_TYPE string("simple")
#define LEFT_UNARY_OPERATOR_TYPE string("lunar")
#define RIGHT_UNARY_OPERATOR_TYPE string("runar")

class Operand : public Expression{
public:
    string GetOperandType() const;
	bool Save(ostream &os) const;
	static bool Load(istream &is,shared_ptr<Operand> &operand_ptr);
};
