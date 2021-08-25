#pragma once

#include <string>
#include <vector>
#include <set>
#include "literal.hpp"

using namespace std;

bool skipToLiteral(int &literalIntex, Literal literal);

bool handleExpressionInstruction(int &literalIntex);

bool handleVarInstruction(int &literalIntex, vector<string> &variableNames);

bool handleIfInstruction(int &literalIntex,
int &condition, int &returnValue, bool &returned);

bool parseBlock(int &literalIntex, int &returnValue, bool &returned);

bool parseVarInstruction(int &literalIntex, int &returnValue, bool &returned,
set<string> &scopeVariables);

bool parseReturnInstruction(int &literalIntex, int &returnValue,
bool &returned);

bool parseFunctionInstruction(int &literalIntex, int &returnValue, bool &returned);

bool parseIfInstruction(int &literalIntex, int &returnValue, bool &returned);

bool parseWhileInstruction(int &literalIntex, int &returnValue, bool &returned);

bool parseInstruction(int &literalIntex, int &returnValue, bool &returned,
set<string> &scopeVariables);

bool executeBlock(int &literalIntex, int &returnValue, bool &returned);

bool executeProgram();
