#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "literal.hpp"
#include "program function.hpp"

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
		/*for (auto functionDescription: functions) {
			if (functionDescription.second.bodyPointer == i) {
				cout << "funciton{" << functionDescription.first << "}";
			}
		} //*/
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

void printFunctions(const map<string,FunctionDescription> &functions,
const vector<Literal> &program) {
	for (auto function : functions) {
		cout << "function " << function.first << " at "
		<< function.second.bodyPointer << " :" << endl;
		const auto &arguments = function.second.arguments;
		for (int i=0; i < arguments.size(); i++) {
			cout << "arg" << i << "=" << arguments[i] << " ";
		}
		cout << endl;
		int bodyPointer = function.second.bodyPointer;
		for (int i = 0; i < 4; i++) {
			cout << program[bodyPointer+i].getValue() << " ";
		}
		cout << endl;
		cout << "FEnd" << endl;
	}
}
