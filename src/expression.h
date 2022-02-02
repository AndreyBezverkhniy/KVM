#pragma once
#include "instruction.h"

#define OPERAND_TYPE string("operand")

class Expression : public Instruction{
public:
    string GetExpressionType() const;
	bool Save(ostream &os) const;
	virtual bool SaveInner(ostream &os) const;
	static bool Load(istream &is,shared_ptr<Expression> &expression_ptr);
};
