#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include "literal.hpp"
#include "program memory.hpp"
#include "program function.hpp"
#include "program read.hpp"
#include "program expression.hpp"

using namespace std;

bool skipToLiteral(int &literalIntex, Literal literal) {
	while (program[literalIntex] != literalEOF) {
		if (program[literalIntex] == literal) {
			return true;
		}
		literalIntex++;
	}
	return false;
}

// handles assignment instruction
bool handleAssignInstruction(int &literalIntex) {

	string variableName = program[literalIntex].getValue();
	if (isNumber(variableName)) {
		cout << "Can not assign new value to a number: " << variableName
		<< endl;
		return false;
	}
	if ( !(doesVariableExist(variableName) || variableName == "CON") ) {
		cout << "LVariable " << variableName << " was not declared" << endl;
		return false;
	}
	literalIntex++; // name

	if (program[literalIntex] != Literal("=",SIGN_LITERAL)) {
		cout << "Assignment pattern expects operator '=' after variable name"
		<< endl;
		return false;
	}
	literalIntex++; // '='

	int result;
	if (!calculateExpression(literalIntex, result)) {
		return false;
	}

	if (program[literalIntex] != Literal(";",SIGN_LITERAL)) {
		cout << "Template define expects ';' at the end" << endl;
		return false;
	}
	literalIntex++; // ";"

	if (variableName == "CON") { // reserved name for console working
		cout << "OUT> " << result << endl;
		return true;
	}
	assignVariable(variableName, result); // usual variable assignment

	return true;

}

// handles variable declaration instruction
bool handleVarInstruction(int &literalIntex, vector<string> &variableNames) {

	literalIntex++; // var

	for (;;) {

		if (program[literalIntex].getType() != WORD_LITERAL) {
			cout << "Template var meets incorrect format of variable name" << endl;
			return false;
		}
		string name = program[literalIntex].getValue();
		variableNames.push_back(name);
		declareVariable(name);
		literalIntex++; // name

		// declaration-assignment
		if (program[literalIntex] == Literal("=",SIGN_LITERAL)) {
			literalIntex++; // '='
			int value;
			calculateExpression(literalIntex, value); // expression
			assignVariable(name, value);
		}

		//
		if (program[literalIntex] == Literal(";",SIGN_LITERAL)) {
			literalIntex++; // ';'
			break;
		}

		if (program[literalIntex] != Literal(",",SIGN_LITERAL)) {
			cout << "Variable declaration pattern expects "
			<< "';' or ',' after name " << name << endl;
			return false;
		}
		literalIntex++; // ','

	}

	return true;

}

// handles if instruction
bool handleIfInstruction(int &literalIntex,
int &condition, int &returnValue, bool &returned) {

	literalIntex++; // if

	if (program[literalIntex+1] != Literal("(",SIGN_LITERAL)) {
		cout << "Expects '(' after if/while" << endl;
		return false;
	}
	literalIntex++; // '('

	// expression
	if (!calculateExpression(literalIntex,condition)) {
		return false;
	}

	if (program[literalIntex] != Literal(")",SIGN_LITERAL)) {
		cout << "Expects ')' after if/while(condition" << endl;
		return false;
	}
	literalIntex++; // ')'

	if (program[literalIntex+1] != Literal("{",SIGN_LITERAL)) {
		cout << "Expects '{' after if/while(condition)" << endl;
		return false;
	}
	literalIntex++; // '{'

	// block
	if (condition) {
		if (!executeBlock(literalIntex, returnValue, returned)) {
			return false;
		}
	} else {
		skipCurrentBlock(literalIntex);
	}

	if (program[literalIntex] != Literal("}",SIGN_LITERAL)) {
		cout << "Expects '}' after block end" << endl;
		return false;
	}
	literalIntex ++; // '}'

	return true;

}

// {...}
bool parseBlock(int &literalIntex, int &returnValue, bool &returned) {

	literalIntex++; // opening '{'

	if (!executeBlock(literalIntex, returnValue, returned)) {
		return false;
	}

	if (program[literalIntex] != Literal("}", SIGN_LITERAL)) {
		cout << "Block end character '}' expected" << endl;
		return false;
	}
	literalIntex++; // closing '}'

	// return instruction floats up
	if (returned) {
		skipCurrentBlock(literalIntex);
	}

	return true;

}

// var <names[with-assigns]>;
bool parseVarInstruction(int &literalIntex, int &returnValue, bool &returned,
set<string> &scopeVariables) {

	vector<string> variableNames;
	if (!handleVarInstruction(literalIntex, variableNames)) {
		return false;
	}

	// Repeated declaration check
	for (auto name : variableNames) {
		if (scopeVariables.find(name) != scopeVariables.end()) {
			cout << "Repeated declaration of variable " << name
			<< " in this scope" << endl;
			return false;
		}
		if (functions.find(name) != functions.end()) {
			cout << "Variable copies name of existing function " << name
			<< "(...)" << endl;
			return false;
		}
		scopeVariables.insert(name);
	}
	
	return true;

}

// return <expression>;
bool parseReturnInstruction(int &literalIntex, int &returnValue,
bool &returned) {

	literalIntex++; // return

	if (!calculateExpression(literalIntex, returnValue)) {
		return false;
	}

	if (program[literalIntex] != Literal(";", SIGN_LITERAL)) {
		cout << "Expects ';' after return <expression>" << endl;
		return false;
	}
	literalIntex++; // ';'

	skipCurrentBlock(literalIntex);
	returned = true;

	return true;

}

// function fname(<arguments>) {...}
bool parseFunctionInstruction(int &literalIntex, int &returnValue, bool &returned) {

	if (!skipToLiteral(literalIntex,Literal("{",SIGN_LITERAL))) {
		return false;
	}
	literalIntex++; // '{'

	skipCurrentBlock(literalIntex);

	if (program[literalIntex] != Literal("}",SIGN_LITERAL)) {
		cout << "Function body expect '}' at the end" << endl;
		return false;
	}
	literalIntex++; // '}'

	return true;

}

// if (<expression>) {...}
bool parseIfInstruction(int &literalIntex, int &returnValue, bool &returned) {

	int condition;

	if (!handleIfInstruction(literalIntex, condition,
	returnValue, returned)) {
		return false;
	}

	// return instruction floats up
	if (returned) {
		skipCurrentBlock(literalIntex);
	}

	return true;

}

// while (<expression>) {...}
bool parseWhileInstruction(int &literalIntex, int &returnValue, bool &returned) {

	int condition;
	int circleIntex = literalIntex;
	do {

		literalIntex = circleIntex;
		if (!handleIfInstruction(literalIntex, condition,
		returnValue, returned)) {
			return false;
		}

		// return instruction floats up
		if (returned) {
			skipCurrentBlock(literalIntex);
			break;
		}

	} while (condition);

	return true;

}

// parse one instruction
// scopeVariables - variables declared in current (deepest) block
bool parseInstruction(int &literalIntex, int &returnValue, bool &returned,
set<string> &scopeVariables) {
	if (program[literalIntex] == Literal("{", SIGN_LITERAL)) {
		parseBlock(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("var",WORD_LITERAL)) {
		parseVarInstruction(literalIntex, returnValue, returned,
		scopeVariables);
	} else 	if (program[literalIntex] == Literal("return",WORD_LITERAL)) {
		parseReturnInstruction(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("function",WORD_LITERAL)) {
		parseFunctionInstruction(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("if",WORD_LITERAL)) {
		parseIfInstruction(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("while",WORD_LITERAL)) {
		parseWhileInstruction(literalIntex, returnValue, returned);
	} else if (program[literalIntex].getType() != WORD_LITERAL) {
		cout << "Unexpected non-word literal: "
		<< program[literalIntex].getValue() << endl;
		return false;
	} else {
		// name=<expression>;
		if (!handleAssignInstruction(literalIntex)) {
			return false;
		}
	}
	return true;
}

// starts program execution from literalIntex
// returns true if success
bool executeBlock(int &literalIntex, 
int &returnValue, bool &returned) {
	returned = false;
	set<string> scopeVariables; // variables created in block scope
	while (program[literalIntex] != literalEOF
	&& program[literalIntex] != Literal("}", SIGN_LITERAL)) {
		if ( !parseInstruction(literalIntex, returnValue, returned,
		scopeVariables) ) {
			return false;
		}
	}
	for (auto variable : scopeVariables) {
		deleteVariable(variable);
	}
	return true;
}

// it's just shell for executeBlock function to execute whole program as block
bool executeProgram() {
	cout << endl << "EXECUTION" << endl << endl;
	int literalIntex = 0;
	int returnValue;
	bool returned;
	bool success = executeBlock(literalIntex, returnValue, returned);
	cout << endl;
	if (returned) {
		cout << "return " << returnValue << endl;
	}
	if (success) {
		cout << "EXECUTION " << "FINISHED" << endl;
	} else {
		cout << "EXECUTION " << "FINISHED at: literalIntex=" << literalIntex << endl;
	}
	return success;
}
