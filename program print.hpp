#pragma once

#include <iostream>
#include <vector>
#include "literal.hpp"

using namespace std;

void printIndent(int indent) {
	for (int i=0; i< indent; i++) {
		cout << "  ";
	}
}

void printProgram(const vector<Literal> &program, bool highlighting = false) {
	int indent = 0;
	bool instructionStarts = true;
	for (int i = 0; i < program.size(); i++ ) {
		if (instructionStarts) {
			if (program[i] == Literal("}", SIGN_LITERAL)) {
				printIndent(indent - 1);
			} else {
				printIndent(indent);
			}
			instructionStarts = false;
		}
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
		if (program[i] == Literal(";",SIGN_LITERAL)
		|| program[i] == Literal("{",SIGN_LITERAL)
		|| program[i] == Literal("}",SIGN_LITERAL)) {
			instructionStarts = true;
			cout << endl;
		}
		if (program[i] == Literal("{",SIGN_LITERAL)) {
			indent++;
		}
		if (program[i] == Literal("}",SIGN_LITERAL)) {
			indent--;
		}
	}
	cout << endl;
}
