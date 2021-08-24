#pragma once

#include <string>
#include <vector>

using namespace std;

bool isNumber(string word);

int toNumber(string word);

bool parseFunctionValue(int &literalIntex, int &result);

bool parseLiteralValue(int &literalIntex, int &result);


bool parseOperand(int &literalIntex, int &operand);

int getOperatorPriority(string math_operator);

bool isOperator(string _operator);

bool calculate(int operand1, int operand2, string math_operator, int &result);

bool parseOperator(int &literalIntex, string &math_operator);

bool parseExpression(int &literalIntex,
vector<int> &operands, vector<string> &operators);

bool twoStackAlgorithm(const vector<int> &operands,
const vector<string> &operators, int &result);

bool calculateExpression(int &literalIntex, int &result);
