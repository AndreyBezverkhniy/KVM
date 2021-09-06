#include "execute_program.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include "literal.hpp"
#include "memory.hpp"
#include "function.hpp"
#include "expression.hpp"
#include "utils.hpp"
#include "operand_object.hpp"

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
bool handleExpressionInstruction(int &literalIntex) {

	OperandObject result;
	if (!calculateExpression(literalIntex, result)) { // expression
		return false;
	}

	if (!parseExactLiteral(literalIntex, ";")) { // ';'
		return false;
	}

	return true;

}

// handles variable declaration instruction
bool handleVarInstruction(int &literalIntex, vector<string> &variableNames) {

	literalIntex++; // var

	for (;;) {

		if (program[literalIntex].getType() != WORD_LITERAL) {
			cout << "Template var meets incorrect format of variable name"
			<< endl;
			return false;
		}
		string name = program[literalIntex].getValue();
		variableNames.push_back(name);
		if (!isUserDefinedNamePermitted(name)) {
			cout << "user defined name " << name << " is not permitted"
			<< endl;
			return false;
		}
		declareVariable(name);
		literalIntex++; // name

		// declaration-assignment
		if (program[literalIntex] == Literal("=")) {
			literalIntex++; // '='
			OperandObject expression;
			calculateExpression(literalIntex, expression); // expression
			assignVariable(name, expression.value);
		}

		if (parseExactLiteral(literalIntex, ";", false)) { // ';'
			break; // no more variable declarations
		}

		if (!parseExactLiteral(literalIntex, ",")) { // ','
			return false;
		}

	}

	return true;

}

// handles if instruction
bool handleIfInstruction(int &literalIntex,
int &condition, int &returnValue, bool &returned) {

	literalIntex++; // if

	if (!parseExactLiteral(literalIntex, "(")) { // '('
		return false;
	}

	// expression
	OperandObject result;
	if (!calculateExpression(literalIntex, result)) {
		return false;
	}
	condition = result.value;

	if (!parseExactLiteral(literalIntex, ")")) { // ')'
		return false;
	}

	if (!parseExactLiteral(literalIntex, "{")) { // '{'
		return false;
	}

	// block body
	if (condition) {
		if (!executeBlock(literalIntex, returnValue, returned)) {
			return false;
		}
	} else {
		skipCurrentBlock(literalIntex);
	}

	if (!parseExactLiteral(literalIntex, "}")) { // '}'
		return false;
	}

	return true;

}

// {...}
bool parseBlock(int &literalIntex, int &returnValue, bool &returned) {

	if (!parseExactLiteral(literalIntex, "{")) { // '{'
		return false;
	}

	// block body
	if (!executeBlock(literalIntex, returnValue, returned)) {
		return false;
	}

	if (!parseExactLiteral(literalIntex, "}")) { // '}'
		return false;
	}

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

	OperandObject returnObject;
	if (!calculateExpression(literalIntex, returnObject)) {
		return false;
	}
	returnValue = returnObject.value;

	if (!parseExactLiteral(literalIntex, ";")) { // ';'
		return false;
	}

	skipCurrentBlock(literalIntex);
	returned = true;

	return true;

}

// function fname(<arguments>) {...}
bool parseFunctionInstruction(int &literalIntex, int &returnValue, bool &returned) {

	skipToLiteral(literalIntex,Literal("{"));
	if (!parseExactLiteral(literalIntex, "{")) { // '{'
		return false;
	}

	skipCurrentBlock(literalIntex); // block body skipped

	if (!parseExactLiteral(literalIntex, "}")) { // '}'
		return false;
	}

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
	if (program[literalIntex] == Literal("{")) {
		return parseBlock(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("var")) {
		return parseVarInstruction(literalIntex, returnValue, returned,
		scopeVariables);
	} else 	if (program[literalIntex] == Literal("return")) {
		return parseReturnInstruction(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("function")) {
		return parseFunctionInstruction(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("if")) {
		return parseIfInstruction(literalIntex, returnValue, returned);
	} else 	if (program[literalIntex] == Literal("while")) {
		return parseWhileInstruction(literalIntex, returnValue, returned);
	} else {
		// <expression>;
		return handleExpressionInstruction(literalIntex);
	}
}

// starts program execution from literalIntex
// returns true if success
bool executeBlock(int &literalIntex, int &returnValue, bool &returned) {
	returned = false;
	set<string> scopeVariables; // variables created in block scope
	while (program[literalIntex] != literalEOF
	&& program[literalIntex] != Literal("}")) {
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
