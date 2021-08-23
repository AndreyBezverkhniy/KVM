#pragma once

bool parseExactLiteral(int &literalIntex, Literal literal,
bool printError = true) {
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
bool printError = true) {
    return parseExactLiteral(literalIntex, Literal(value), printError);
}
