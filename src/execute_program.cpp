#include "execute_program.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include "literal.hpp"
#include "memory.hpp"
#include "function.hpp"
#include "inbuilt_functions.hpp"
#include "expression.hpp"
#include "utils.hpp"
#include "operand_object.hpp"
#include "errors.hpp"

using namespace std;

bool skipToLiteral(int &literalIndex, Literal literal) {
	while (program[literalIndex] != literalEOF) {
		if (program[literalIndex] == literal) {
			return true;
		}
		literalIndex++;
	}
	return false;
}

// handles assignment instruction
bool handleExpressionInstruction(int &literalIndex) {

	OperandObject result;
	if (!calculateExpression(literalIndex, result)) { // expression
		return false;
	}

	if (!parseExactLiteral(literalIndex, ";")) { // ';'
		showErrorMessage(errorInstructionPattern, ";");
		return false;
	}

	return true;

}

// handles variable declaration instruction
bool handleVarInstruction(int &literalIndex, vector<string> &variableNames) {

	literalIndex++; // var

	for (;;) {

		if (program[literalIndex].getType() != WORD_LITERAL) {
			showErrorMessage(errorVarPattern, "variable name");
			return false;
		}
		string name = program[literalIndex].getValue();
		variableNames.push_back(name);
		if (!isUserDefinedNamePermitted(name)) {
			showErrorMessage(errorUserDefinedName, name);
			return false;
		}
		declareVariable(name);
		literalIndex++; // name

		// declaration-assignment
		if (program[literalIndex] == Literal("=")) {
			literalIndex++; // '='
			OperandObject expression;
			calculateExpression(literalIndex, expression); // expression
			assignVariable(name, expression.value);
		}

		if (parseExactLiteral(literalIndex, ";", false)) { // ';'
			break; // no more variable declarations
		}

		if (!parseExactLiteral(literalIndex, ",")) { // ','
			showErrorMessage(errorVarPattern, "','");
			return false;
		}

	}

	return true;

}

// handles if instruction
bool handleIfInstruction(int &literalIndex,
int &condition, int &returnValue, bool &returned) {

	literalIndex++; // if

	if (!parseExactLiteral(literalIndex, "(")) { // '('
		showErrorMessage(errorIfPattern, "'('");
		return false;
	}

	// expression
	OperandObject result;
	if (!calculateExpression(literalIndex, result)) {
		return false;
	}
	condition = result.value;

	if (!parseExactLiteral(literalIndex, ")")) { // ')'
		showErrorMessage(errorIfPattern, "')'");
		return false;
	}

	if (!parseExactLiteral(literalIndex, "{")) { // '{'
		showErrorMessage(errorIfPattern, "'{'");
		return false;
	}

	// block body
	if (condition) {
		if (!executeBlock(literalIndex, returnValue, returned)) {
			return false;
		}
	} else {
		skipCurrentBlock(literalIndex);
	}

	if (!parseExactLiteral(literalIndex, "}")) { // '}'
		showErrorMessage(errorIfPattern, "'}'");
		return false;
	}

	return true;

}

// {...}
bool parseBlock(int &literalIndex, int &returnValue, bool &returned) {

	if (!parseExactLiteral(literalIndex, "{")) { // '{'
		showErrorMessage(errorBlockPattern, "'{'");
		return false;
	}

	// block body
	if (!executeBlock(literalIndex, returnValue, returned)) {
		return false;
	}

	if (!parseExactLiteral(literalIndex, "}")) { // '}'
		showErrorMessage(errorBlockPattern, "'}'");
		return false;
	}

	// return instruction floats up
	if (returned) {
		skipCurrentBlock(literalIndex);
	}

	return true;

}

// var <names[with-assigns]>;
bool parseVarInstruction(int &literalIndex, int &returnValue, bool &returned,
set<string> &scopeVariables) {

	vector<string> variableNames;
	if (!handleVarInstruction(literalIndex, variableNames)) {
		return false;
	}

	// Repeated declaration check
	for (auto name : variableNames) {
		if (scopeVariables.find(name) != scopeVariables.end()) {
			showErrorMessage(errorRepeatedVariableDeclaration, name);
			return false;
		}
		if (doesFunctionExist(name) || doesInbuiltFunctionExist(name)) {
			showErrorMessage(errorFunctionOverloadByVariable, name);
			return false;
		}
		scopeVariables.insert(name);
	}
	
	return true;

}

// return <expression>;
bool parseReturnInstruction(int &literalIndex, int &returnValue,
bool &returned) {

	literalIndex++; // return

	OperandObject returnObject;
	if (!calculateExpression(literalIndex, returnObject)) {
		return false;
	}
	returnValue = returnObject.value;

	if (!parseExactLiteral(literalIndex, ";")) { // ';'
		showErrorMessage(errorReturnPattern, "';'");
		return false;
	}

	skipCurrentBlock(literalIndex);
	returned = true;

	return true;

}

// function fname(<arguments>) {...}
bool parseFunctionInstruction(int &literalIndex, int &returnValue,
bool &returned) {

	skipToLiteral(literalIndex,Literal("{"));
	if (!parseExactLiteral(literalIndex, "{")) { // '{'
		showErrorMessage(errorFunctionDeclarationPattern, "'{'");
		return false;
	}

	skipCurrentBlock(literalIndex); // block body skipped

	if (!parseExactLiteral(literalIndex, "}")) { // '}'
		showErrorMessage(errorFunctionDeclarationPattern, "'}'");
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
bool parseInstruction(int &literalIndex, int &returnValue, bool &returned,
set<string> &scopeVariables) {
	string literalValue = program[literalIndex].getValue();
	if (literalValue == "{") {
		return parseBlock(literalIndex, returnValue, returned);
	} else 	if (literalValue == "var") {
		return parseVarInstruction(literalIndex, returnValue, returned,
		scopeVariables);
	} else 	if (literalValue == "return") {
		return parseReturnInstruction(literalIndex, returnValue, returned);
	} else 	if (literalValue == "function") {
		return parseFunctionInstruction(literalIndex, returnValue, returned);
	} else 	if (literalValue == "if") {
		return parseIfInstruction(literalIndex, returnValue, returned);
	} else 	if (literalValue == "while") {
		return parseWhileInstruction(literalIndex, returnValue, returned);
	} else {
		// <expression>;
		return handleExpressionInstruction(literalIndex);
	}
}

// starts program execution from literalIndex
// returns true if success
bool executeBlock(int &literalIndex, int &returnValue, bool &returned) {
	returned = false;
	set<string> scopeVariables; // variables created in block scope
	while (program[literalIndex] != literalEOF
	&& program[literalIndex] != Literal("}")) {
		if ( !parseInstruction(literalIndex, returnValue, returned,
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
	int literalIndex = 0;
	int returnValue;
	bool returned;
	bool success = executeBlock(literalIndex, returnValue, returned);
	cout << endl;
	if (returned) {
		cout << "return " << returnValue << endl;
	}
	if (success) {
		cout << "EXECUTION " << "FINISHED" << endl;
	} else {
		cout << "EXECUTION " << "FAILED at: literalIndex=" << literalIndex << endl;
	}
	return success;
}
