#pragma once
#include "expression.h"

#define ELEMENT_TYPE string("element")
#define LEFT_UNARY_OPERATOR_TYPE string("lunar")

class Operand : public Expression{
public:
    string GetOperandType() const;
	bool Save(ostream &os) const;
	virtual bool SaveInner(ostream &os) const;
	static bool Load(istream &is,shared_ptr<Operand> &operand_ptr);
};
