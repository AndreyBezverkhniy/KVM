#include "utils.hpp"
#include "memory.hpp"
#include <iostream>
#include <vector>

using namespace std;

bool isDigit(char character) {
	return character >= '0' && character <='9';
}

// chech if word represents unsigned int number
bool isNumber(string word) {
	if (word.size() == 0 || word.size()>1 && word[0] == '0') {
		return false;
	}
	for (auto ch : word) {
		if (ch < '0' || ch > '9') {
			return false;
		}
	}
	return true;
}

int toNumber(string word) {
	int result=0;
	for (int i=0; i < word.size(); i++) {
		result *= 10;
		result += word[i] - '0';
	}
	return result;
}

void skipCurrentBlock(int &literalIntex) {
	int blockDeep = 1; // deep of nesting blocks
	while (program[literalIntex] != literalEOF) {
		if (program[literalIntex] == Literal("{")) {
			blockDeep++;
		} else if (program[literalIntex] == Literal("}")) {
			blockDeep--;
		}
		if (blockDeep == 0) {
			// whole block (with nested ones) is skipped
			break;
		}
		literalIntex++;
	}
}

bool parseExactLiteral(int &literalIntex, Literal literal,
bool printError) {
	if (program[literalIntex] == literal) {
		literalIntex++;
	    return true;
	}
	if (printError) {
		cout << "Expected " << literal.getValue() << endl;
	}
	return false;
}

bool parseExactLiteral(int &literalIntex, string value,
bool printError) {
    return parseExactLiteral(literalIntex, Literal(value), printError);
}

bool isUserDefinedNamePermitted(string name) {
	return !isDigit(name[0]);
}

void showErrorMessage(string errorMessage, string parameter) {
	cout << errorMessage << ": " << parameter << endl;
}
