#pragma once

#include <iostream>
#include <vector>
#include "literal.hpp"

using namespace std;

void printProgram(const vector<Literal> &program, bool highlighting = false) {
	for (int i = 0; i < program.size(); i++ ) {
		if (highlighting) {
			cout << "<";
		}
		if (program[i].getType() == STRING_LITERAL) {
			cout << "\"";
		}
		cout << program[i].getValue();
		if (program[i].getType() == STRING_LITERAL) {
			cout << "\"";
		}
		if (highlighting) {
			cout << ">";
		}
		cout << " ";
		if (program[i] == Literal(";",SIGN_LITERAL) ||
		program[i] == Literal("{",SIGN_LITERAL) ||
		program[i] == Literal("}",SIGN_LITERAL)) {
			cout << endl;
		}
	}
	cout << endl;
}
