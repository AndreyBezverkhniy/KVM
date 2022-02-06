#pragma once
#include <vector>
#include "literal.h"
#include "expression.h"
#include "number.h"
#include "operand.h"
#include "bin_operator.h"
#include <string>

using namespace std;

bool ReadExpression(const vector<Literal> &vec,int &index,shared_ptr<Expression> &expression);
bool ReadBinOperator(const vector<Literal> &vec,int &index,string &bin);
bool ReadOperand(const vector<Literal> &vec,int &index,shared_ptr<Operand> &operand);
bool ReadNumber(const vector<Literal> &vec,int &index,shared_ptr<Number> &number);
int GetBinOperandPriority(string bin);
bool IsRightAssociativeBinOperator(string bin);
bool BinOrder(string binl,string binr);
shared_ptr<Expression> BuildExpression(vector<shared_ptr<Operand>> &ops,vector<string> &bins);
