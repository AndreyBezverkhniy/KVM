#pragma once
#include "instruction.h"

#define OPERAND_TYPE string("operand")
#define BIN_OPERATOR_TYPE string("bin")

class Expression : public Instruction{
public:
    string GetExpressionType() const;
	bool Save(ostream &os) const;
	static bool Load(istream &is,shared_ptr<Expression> &expression_ptr);
};
