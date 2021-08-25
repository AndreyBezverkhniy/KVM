#pragma once

#include <string>
#include <vector>
#include "operand_object.hpp"

using namespace std;

bool isNumber(string word);

int toNumber(string word);

bool parseFunctionValue(int &literalIntex, OperandObject &result);

bool parseLiteralValue(int &literalIntex, OperandObject &result);


bool parseOperand(int &literalIntex, OperandObject &operand);

int getOperatorPriority(string math_operator);

bool isOperator(string _operator);

bool calculate(OperandObject operand1, OperandObject operand2,
string math_operator, OperandObject &result);

bool parseOperator(int &literalIntex, string &math_operator);

bool parseExpression(int &literalIntex,
vector<OperandObject> &operands, vector<string> &operators);

bool twoStackAlgorithm(const vector<OperandObject> &operands,
const vector<string> &operators, int &result);

bool calculateExpression(int &literalIntex, OperandObject &result);
