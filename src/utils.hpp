#pragma once

#include "literal.hpp"
#include <vector>
#include <algorithm>

void skipCurrentBlock(int &literalIntex);

template<class T> bool hasVectorAnElement(vector<T> &array, T element) {
	return find(array.begin(), array.end(), element) != array.end();
}

bool parseExactLiteral(int &literalIntex, Literal literal,
bool printError = true);

bool parseExactLiteral(int &literalIntex, string value,
bool printError = true);
