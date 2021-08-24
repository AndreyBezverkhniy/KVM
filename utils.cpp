#include "utils.hpp"
#include "memory.hpp"
#include <iostream>

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
