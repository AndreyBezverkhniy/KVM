#pragma once
#include "operand.h"
#include <memory>

class UnaryOperator : public Operand{
public:
    shared_ptr<Expression> operand;
    string operation;
	UnaryOperator();
    void SetOperand(shared_ptr<Expression> operand);
    void SetOperation(string operation);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
