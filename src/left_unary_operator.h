#pragma once
#include "operand.h"
#include <memory>

#define LEFT_UNARY_PLUS string("+")
#define LEFT_UNARY_MINUS string("-")
#define LEFT_UNARY_INC string("++")
#define LEFT_UNARY_DEC string("--")
#define LEFT_UNARY_NOT string("!")

class LeftUnaryOperator : public Operand{
public:
    shared_ptr<Operand> operand;
    string operation;
	LeftUnaryOperator();
    void SetOperand(shared_ptr<Operand> operand);
    void SetOperation(string operation);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
