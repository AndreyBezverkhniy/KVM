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

void printFunctions();

void printProgram(bool highlighting = false) {
	int indent = 0;
	bool instructionStarts = true;
	for (int i = 0; i < program.size(); i++ ) {
		if (instructionStarts) {
			if (program[i] == Literal("}")) {
				printIndent(indent - 1);
			} else {
				printIndent(indent);
			}
			instructionStarts = false;
		}
		/*for (auto functionDescription: functions) {
			if (functionDescription.second.bodyIntex == i) {
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
	printFunctions();
}

void printFunctions() {
	for (auto NameDescription : functions) {
		string functionName = NameDescription.first;
		FunctionDescription functionDescription = NameDescription.second;
		cout << "function " << functionName << " at "
		<< functionDescription.bodyIntex << " :" << endl;
		const auto arguments = functionDescription.arguments;
		for (int i=0; i < arguments.size(); i++) {
			cout << "arg" << i << "=" << arguments[i] << " ";
		}
		cout << endl;
		int bodyIntex = functionDescription.bodyIntex;
		for (int i = 0; i < 4; i++) {
			cout << program[bodyIntex+i].getValue() << " ";
		}
		cout << endl;
		cout << "F-End" << endl;
	}
}
