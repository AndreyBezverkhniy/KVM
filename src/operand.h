#pragma once
#include "expression.h"

#define LEFT_UNARY_OPERATOR_TYPE "lunar"
#define RIGHT_UNARY_OPERATOR_TYPE "runar"

class Operand : public Expression{
public:
    string GetType() const override;
	static bool IsOperand(string type);
	static bool Load(istream &is,shared_ptr<Operand> &operand_ptr);
	static bool LoadTyped(string type,istream &is,shared_ptr<Operand> &operand_ptr);
};
