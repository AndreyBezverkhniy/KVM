#include "print_program.hpp"

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "literal.hpp"
#include "function.hpp"
#include "memory.hpp"

using namespace std;

void printIndent(int indent) {
	for (int i=0; i < indent; i++) {
		cout << "\t";
	}
}

void printFunctions() {

	for (auto NameDescription : functions) {

		string functionName = NameDescription.first;
		FunctionDescription functionDescription = NameDescription.second;

		cout << "function " << functionName << " at "
		<< functionDescription.bodyIntex << ", arguments :" << endl;

		const auto arguments = functionDescription.arguments;
		for (int i=0; i < arguments.size(); i++) {
			cout << "arg" << i << "=" << arguments[i] << " ";
		}

		cout << endl;

		// several literals from function body start
		int bodyIntex = functionDescription.bodyIntex;
		for (int i = 0; i < 7; i++) {
			cout << program[bodyIntex+i].getValue() << " ";
		}
		cout << "..." << endl;

	}

}

void printProgram(bool highlighting) {

	int indent = 1;
	bool instructionStarts = true; // for indent printing

	for (int i = 0; i < program.size(); i++ ) {

		// indents before instruction starts
		if (instructionStarts) {
			cout << i << ":";
			if (program[i] == Literal("}")) {
				printIndent(indent - 1);
			} else {
				printIndent(indent);
			}
			instructionStarts = false;
		}

		// print literal with(-out) highlighting
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

		cout << " "; // to separate literals

		// end of instruction
		if (program[i] == Literal(";")
		|| program[i] == Literal("{")
		|| program[i] == Literal("}")) {
			instructionStarts = true; // new instruction expected farther
			cout << endl;
		}

		if (program[i] == Literal("{")) {
			indent++;
		}

		if (program[i] == Literal("}")) {
			indent--;
		}

	}
	cout << endl;

	printFunctions();

}
