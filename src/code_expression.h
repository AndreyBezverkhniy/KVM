#pragma once
#include <vector>
#include "literal.h"
#include "expression.h"
#include "bin_operator.h"
#include "operand.h"
#include "number.h"
#include "variable_name.h"
#include <string>

using namespace std;

bool ReadExpression(const vector<Literal> &vec,int &index,shared_ptr<Expression> &expression);
bool ReadBinOperator(const vector<Literal> &vec,int &index,string &bin);
bool ReadOperand(const vector<Literal> &vec,int &index,shared_ptr<Expression> &operand);
bool ReadSimpleExpression(const vector<Literal> &vec,int &index,
shared_ptr<Expression> &simple_expression);
bool ReadNumber(const vector<Literal> &vec,int &index,shared_ptr<Number> &number);
bool ReadVariableName(const vector<Literal> &vec,int &index,shared_ptr<VariableName> &variable);
bool ReadParenthesizedExpression(const vector<Literal> &vec,int &index,
shared_ptr<Expression> &parenthesized);
int GetBinOperandPriority(string bin);
bool IsRightAssociativeBinOperator(string bin);
bool BinOrder(string binl,string binr);
shared_ptr<Expression> BuildExpression(vector<shared_ptr<Expression>> &ops,vector<string> &bins);
