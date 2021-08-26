#include "utils.hpp"
#include "memory.hpp"
#include <iostream>
#include <vector>

using namespace std;

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
