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

using namespace std;

bool calculateExpression(int &literalIntex, int &result);

bool executeBlock(int &literalIntex, int &returnValue, bool &returned);

// chech if word represents unsigned int number
bool isNumber(string word) {
	if (word.size() == 0 || word.size()>1 && word[0] == '0') {
		return false;
	}
	for (auto ch : word) {
		if (ch < '0' || ch > '9') {
			return false;
		}
	}
	return true;
}

int toNumber(string word) {
	int result=0;
	for (int i=0; i < word.size(); i++) {
		result *= 10;
		result += word[i] - '0';
	}
	return result;
}

bool parseFunctionValue(int &literalIntex, int &result) {
	string functionName = program[literalIntex].getValue();
	FunctionDescription &description = functions[functionName];
	literalIntex++; // functionName
	if (program[literalIntex] != Literal("(",SIGN_LITERAL)) {
		cout << "Expected '(' after function call: " << functionName << endl;
		return false;
	}
	literalIntex++; // (
	int argumentNumber = 0;
	vector<int> argumentsPassed;
	while (argumentNumber < description.arguments.size()) {
		if (argumentNumber > 0) {
			if (program[literalIntex] != Literal(",",SIGN_LITERAL)) {
				cout << "Expected ',' between arguments in call of function: "
				<< functionName << endl;
				return false;
			}
			literalIntex++; // ,
		}
		int argumentPassed;
		if (!calculateExpression(literalIntex, argumentPassed)) {
			return false;
		}
		argumentsPassed.push_back(argumentPassed);
		argumentNumber++;
	}
	if (program[literalIntex] != Literal(")",SIGN_LITERAL)) {
		cout << "Expected ')' after function call: " << functionName << endl;
		return false;
	}
	literalIntex++; // )
	for (int argumentIndex = 0; argumentIndex < argumentsPassed.size();
	argumentIndex++) {
		string argumentName = description.arguments[argumentIndex];
		declareVariable(argumentName);
		assignVariable(argumentName,argumentsPassed[argumentIndex]);
	}
	int savedIntex = literalIntex;
	literalIntex = description.bodyIntex;
	result = 0; // by default
	bool garbage;
	if (!executeBlock(literalIntex,result,garbage)) {
		return false;
	}
	literalIntex = savedIntex;
	for (auto argumentName : description.arguments) {
		deleteVariable(argumentName);
	}
	return true;
}

// handles numbers and variables
// saves result to corresponding argument-variable
bool parseLiteralValue(int &literalIntex,
int &result) {
	string word = program[literalIntex].getValue();
	if (isNumber(word)) {
		result = toNumber(word);
		literalIntex++; // number
		return true;
	}
	if (word == "CON") { // reserved name for console working
		cout << "IN> ";
		cin >> result;
		literalIntex++; // CON
		return true;
	}
	if (functions.find(word) != functions.end()) {
		return parseFunctionValue(literalIntex, result);
	}
	if (!doesVariableExist(word)) {
		cout << "Variable " << word << " was not declared" << endl;
		return false;
	}
	result = getVariableValue(word);
	literalIntex++; // variable
	return true;
}

bool parseOperand(int &literalIntex, int &operand) {
	if (program[literalIntex].getType() == WORD_LITERAL) {
		// number or variable
		if (!parseLiteralValue(literalIntex, operand)) {
			return false;
		}
		return true;
	} else if (program[literalIntex] == Literal("(",SIGN_LITERAL)) {
		// calculating expression in brackets to just number operand
		literalIntex++; // opening '('
		int expressionValue;
		if (!calculateExpression(literalIntex, expressionValue)) {
			return false;
		}
		if (program[literalIntex] != Literal(")", SIGN_LITERAL)) {
			cout << "Not complited patherness sequence" << endl;
			return false;
		}
		literalIntex++; // closing ')'
		operand = expressionValue;
		return true;
	}
	cout << "Wrong operand: " << program[literalIntex].getValue() << endl;
	return false;
}

int getOperatorPriority(string math_operator) {
	if (math_operator == "||") {
		return 1;
	}
	if (math_operator == "&&") {
		return 2;
	}
	if (math_operator == "==" || math_operator == "!=" || math_operator == ">="
	|| math_operator == "<=" || math_operator == ">" || math_operator == "<") {
		return 3;
	}
	if (math_operator == "+" || math_operator == "-") {
		return 4;
	}
	if (math_operator == "*" || math_operator == "/" || math_operator == "%") {
		return 5;
	}
	return 0;
}

bool calculate(int operand1, int operand2, string math_operator, int &result) {
	if (math_operator == "||") {
		result = operand1 || operand2;
	} else if (math_operator == "&&") {
		result = operand1 && operand2;
	} else if (math_operator == "==") {
		result = operand1 == operand2;
	} else if (math_operator == "!=") {
		result = operand1 != operand2;
	} else if (math_operator == ">=") {
		result = operand1 >= operand2;
	} else if (math_operator == "<=") {
		result = operand1 <= operand2;
	} else if (math_operator == ">") {
		result = operand1 > operand2;
	} else if (math_operator == "<") {
		result = operand1 < operand2;
	} else if (math_operator == "+") {
		result = operand1 + operand2;
	} else if (math_operator == "-") {
		result = operand1 - operand2;
	} else if (math_operator == "*") {
		result = operand1 * operand2;
	} else if (math_operator == "/") {
		result = operand1 / operand2;
	} else if (math_operator == "%") {
		result = operand1 % operand2;
	} else {
		return false;
	}
	return true;
}

bool parseOperator(int &literalIntex, string &math_operator) {
	if (program[literalIntex].getType() == SIGN_LITERAL){
		string sign = program[literalIntex].getValue();
		if (getOperatorPriority(sign) != 0) {
			math_operator = sign;
			literalIntex++;
			return true;
		}
	}
	return false;
}

// calculates expressions in brackets, then separates simplified expression to
// operators (ints) and operands
bool parseExpression(int &literalIntex,
vector<int> &operands, vector<string> &operators) {
	// expression contain math signs, correct bracket sequences, numbers,
	// variables. ends by ';', ')', ',' signs and if vector ends
	for(;;){
		int operand;
		string math_operator;
		// if operand is bracket with expression inside, it calculates and
		// result is used as operand
		if (!parseOperand(literalIntex, operand)) {
			return false;
		}
		if (!parseOperator(literalIntex, math_operator)) {
			// check end of expression
			if (literalIntex == program.size()
			|| program[literalIntex].getValue() == ";"
			|| program[literalIntex].getValue() == ")"
			|| program[literalIntex].getValue() == ",") {
				operands.push_back(operand);
				operators.push_back(";"); // considered as lowest priority
				// empty operator
				break; // expression parsed successfully
			}
			// invalid operator
			cout << "Incorrect operator: " << program[literalIntex].getValue() << endl;
			return false;
		}
		operands.push_back(operand);
		operators.push_back(math_operator);
	}
	return true;
}

// calculates value of math expression without brackets
// expression is separated to arrays of operands and operators
// saves result to corresponding argument-variable
bool twoStackAlgorithm(const vector<int> &operands,
const vector<string> &operators, int &result) {
	stack<int> operandStack;
	stack<string> operatorStack;
	// last operand and operator in stacks are separated to variables for
	// availability of its prenultimate ones
	int currentOperand = operands[0];
	string currentOperator = operators[0];

	for (int i=1; i < operands.size(); i++) {
		// put next operand and operator to stacks
		operandStack.push(currentOperand);
		operatorStack.push(currentOperator);
		currentOperand = operands[i];
		currentOperator = operators[i];
		// applying previous operators with higher priority than the last
		// one in reverse order
		while (operatorStack.size() > 0 &&
		getOperatorPriority(operatorStack.top())
		>= getOperatorPriority(currentOperator)) {
			int operationResult;
			int operand1 = operandStack.top();
			operandStack.pop();
			int operand2 = currentOperand;
			string math_operator = operatorStack.top();
			operatorStack.pop();
			if (!calculate(operand1,operand2,math_operator,operationResult)) {
				cout << "Calculation error: " << operand1 << math_operator << operand2 << endl;
				return false;
			}
			currentOperand = operationResult;
		}
	}
	result = currentOperand;
	return true;
}

// gets values of expressions in brackets
// then impliments two-stack-algorithm to math expression without brackets
// saves result to corresponding argument-variable
bool calculateExpression(int &literalIntex, int &result) {
	// preparing expression for algorithm: separating operands from operators
	// and calculating expresions in brackets
	vector<int> operands;
	vector<string> operators;
	if (!parseExpression(literalIntex,operands,operators)) {
		return false;
	}
	if (operands.size() == 1) {
		// expression consists only value or varible
		result = operands[0];
		return true;
	}
	// two stack algorithm to calculate math expressions without brackets
	// brackets are calculated while parsing expression to operands and
	// operators
	return twoStackAlgorithm(operands, operators, result);
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
	if (program[literalIntex + 1] != Literal("=",SIGN_LITERAL)) {
		cout << "Assignment pattern expects operator '=' after variable name"
		<< endl;
		return false;
	}
	literalIntex += 2; // "name ="
	int result;
	if (!calculateExpression(literalIntex, result)) {
		return false;
	}
	if (program[literalIntex] != Literal(";",SIGN_LITERAL)) {
		cout << "Template define expects ';' at the end" << endl;
		return false;
	}
	literalIntex += 1; // ";"
	if (variableName == "CON") { // reserved name for console working
		cout << "OUT> " << result << endl;
		return true;
	}
	assignVariable(variableName, result); // usual variable assignment
	return true;
}

// handles variable declaration instruction
bool handleVarInstruction(int &literalIntex, vector<string> &variableNames) {
	literalIntex++; // WORD(var)
	for (;;) {
		if (program[literalIntex].getType() != WORD_LITERAL) {
			cout << "Template var meets incorrect format of variable name" << endl;
			return false;
		}
		string name = program[literalIntex].getValue();
		variableNames.push_back(name);
		declareVariable(name);
		literalIntex++; // WORD(name)
		if (program[literalIntex] == Literal("=",SIGN_LITERAL)) {
			literalIntex++; // SIGN(=)
			int value;
			calculateExpression(literalIntex, value);
			assignVariable(name, value);
		}
		if (program[literalIntex] == Literal(";",SIGN_LITERAL)) {
			literalIntex++; // SIGN(;)
			break;
		}
		if (program[literalIntex] != Literal(",",SIGN_LITERAL)) {
			cout << "Variable declaration pattern expects "
			<< "';' or ',' after name " << name << endl;
			return false;
		}
		literalIntex++; // SIGN(,)
	}
	return true;
}

bool findFromPosition(int &literalIntex, Literal literal) {
	while (literalIntex < program.size()) {
		if (program[literalIntex] == literal) {
			break;
		}
		literalIntex++;
	}
	return (literalIntex < program.size());
}

// handles if instruction
bool handleIfInstruction(int &literalIntex,
int &condition, int &returnValue, bool &returned) {
	if (program[literalIntex+1] != Literal("(",SIGN_LITERAL)) {
		cout << "Expects '(' after if/while" << endl;
		return false;
	}
	literalIntex += 2; // WORD(if) SIGN('(')
	if (!calculateExpression(literalIntex,condition)) {
		return false;
	}
	if (program[literalIntex] != Literal(")",SIGN_LITERAL)) {
		cout << "Expects ')' after if/while(condition" << endl;
		return false;
	}
	if (program[literalIntex+1] != Literal("{",SIGN_LITERAL)) {
		cout << "Expects '{' after if/while(condition)" << endl;
		return false;
	}
	literalIntex += 2; // SIGN(')') SIGN('{')
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
	literalIntex ++; // SIGN('}')
	return true;
}

// starts program execution from literalIntex
// returns true if success
bool executeBlock(int &literalIntex, 
int &returnValue, bool &returned) {
	returned = false;
	set<string> scopeVariables; // variables created in block witch scope
	// of the block
	while (literalIntex < program.size()
	&& program[literalIntex] != Literal("}", SIGN_LITERAL)) {
		if (program[literalIntex] == Literal("{", SIGN_LITERAL)) {
			// {...}
			literalIntex++; // opening '}'
			if (!executeBlock(literalIntex, returnValue,
			returned)) {
				return false;
			}
			if (program[literalIntex] != Literal("}", SIGN_LITERAL)) {
				cout << "Block end character '}' expected" << endl;
				return false;
			}
			literalIntex++; // closing '}'
			if (returned) {
				skipCurrentBlock(literalIntex);
			}
		} else 	if (program[literalIntex] == Literal("var",WORD_LITERAL)) {
			// var name;
			vector<string> variableNames;
			if (!handleVarInstruction(literalIntex, variableNames)) {
				return false;
			}
			for (auto name : variableNames) {
				if (scopeVariables.find(name) != scopeVariables.end()) {
					cout << "Double declaration of variable " << name
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
		} else 	if (program[literalIntex] == Literal("return",WORD_LITERAL)) {
			// return <expression>;
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
		} else 	if (program[literalIntex] == Literal("function",WORD_LITERAL)) {
			// function fname(<arguments>) {...}
			if (!findFromPosition(literalIntex,Literal("{",SIGN_LITERAL))) {
				return false;
			}
			literalIntex++; // '{'
			skipCurrentBlock(literalIntex);
			if (program[literalIntex] != Literal("}",SIGN_LITERAL)) {
				cout << "Function body expect '}' at the end" << endl;
				return false;
			}
			literalIntex++; // '}'
		} else 	if (program[literalIntex] == Literal("if",WORD_LITERAL)) {
			// if (<expression>) {...}
			int condition;
			if (!handleIfInstruction(literalIntex, condition,
			returnValue, returned)) {
				return false;
			}
			if (returned) {
				skipCurrentBlock(literalIntex);
			}
		} else 	if (program[literalIntex] == Literal("while",WORD_LITERAL)) {
			// while (<expression>) {...}
			int condition;
			int circleIntex = literalIntex;
			do {
				literalIntex = circleIntex;
				if (!handleIfInstruction(literalIntex, condition,
				returnValue, returned)) {
					return false;
				}
				if (returned) {
					skipCurrentBlock(literalIntex);
					break;
				}
			} while (condition);
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
	}
	for (auto variable : scopeVariables) {
		deleteVariable(variable);
	}
	return true;
}

// it's just shell for executeBlock function to execute whole program as block
bool execute() {
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
