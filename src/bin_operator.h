#pragma once
#include "expression.h"
#include <memory>
#include "operand.h"

#define BIN_PLUS string("+")
#define BIN_MINUS string("-")
#define BIN_MULT string("*")
#define BIN_DIV string("/")
#define BIN_MOD string("%")
#define BIN_EQ string("==")
#define BIN_NEQ string("!=")
#define BIN_LOWER string("<")
#define BIN_GREATER string(">")
#define BIN_NGREATER string("<=")
#define BIN_NLOWER string(">=")
#define BIN_AND string("&&")
#define BIN_OR string("||")
#define BIN_ASSIGN string("=")
#define BIN_PLUS_ASSIGN string("+=")
#define BIN_MINUS_ASSIGN string("-=")
#define BIN_MULT_ASSIGN string("*=")
#define BIN_DIV_ASSIGN string("/=")
#define BIN_MOD_ASSIGN string("%=")

class BinOperator : public Expression{
public:
    shared_ptr<Expression> left, right;
    string operation;
	BinOperator();
    void SetLeft(shared_ptr<Expression> left);
    void SetRight(shared_ptr<Expression> right);
    void SetOperation(string operation);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
