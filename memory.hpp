#pragma once

#include <map>
#include <string>
#include <stack>
#include "function.hpp"
#include "literal.hpp"

using namespace std;

extern vector<Literal> program;
extern map<string,FunctionDescription> functions;
extern map<string,stack<int>> variables;

bool doesFunctionExist(string functionName);

bool doesVariableExist(string variableName);

void declareVariable(string variableName);

void declareVariables(vector<string> &variableNames);

void deleteVariable(string variableName);

void deleteVariables(vector<string> &variableNames);

void assignVariable(string variableName, int newValue);

void assignVariables(vector<string> &variableNames, vector<int> &newValues);

int getVariableValue(string variableName);
