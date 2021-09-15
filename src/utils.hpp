#pragma once

#include "literal.hpp"
#include <vector>
#include <algorithm>
#include <string>

bool isDigit(char character);

// chech if word represents unsigned int number
bool isNumber(string word);

int toNumber(string word);

void skipCurrentBlock(int &literalIntex);

template<class T> bool hasVectorAnElement(vector<T> &array, T element) {
	return find(array.begin(), array.end(), element) != array.end();
}

bool parseExactLiteral(int &literalIntex, Literal literal,
bool printError = false);

bool parseExactLiteral(int &literalIntex, string value,
bool printError = false);

bool isUserDefinedNamePermitted(string name);

void showErrorMessage(string errorMessage, string parameter = "");
