#pragma once
#include "expression.h"

#define ELEMENT_TYPE string("element")

class Operand : public Expression{
public:
    string GetOperandType() const;
	bool Save(ostream &os) const;
	virtual bool SaveInner(ostream &os) const;
	static bool Load(istream &is,shared_ptr<Operand> &operand_ptr);
};
