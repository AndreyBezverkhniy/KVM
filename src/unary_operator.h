#pragma once
#include "operand.h"
#include <memory>

class UnaryOperator : public Operand{
public:
    shared_ptr<Operand> operand;
    string operation;
	UnaryOperator();
    void SetOperand(shared_ptr<Operand> operand);
    void SetOperation(string operation);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
