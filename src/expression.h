#pragma once
#include "instruction.h"

#define BIN_OPERATOR_TYPE "bin"

class Expression : public Instruction{
public:
    string GetType() const override;
	static bool IsExpression(string type);
	static bool Load(istream &is,shared_ptr<Expression> &expression_ptr);
	static bool LoadTyped(string type,istream &is,shared_ptr<Expression> &expression_ptr);
};
