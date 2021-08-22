#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "literal.hpp"
#include "program memory.hpp"

using namespace std;

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

// handles numbers and variables
// saves result to corresponding argument-variable
bool getWordValue(string word, int &result) {
	if (isNumber(word)) {
		result = toNumber(word);
		return true;
	}
	if (word == "CON") { // reserved name for console working
		cout << "> ";
		cin >> result;
		return true;
	}
	if (!doesVariableExist(word)) {
		cout << "Variable " << word << " was not declared" << endl;
		return false;
	}
	result = getVariableValue(word);
	return true;
}

bool calculateExpression(vector<Literal> &program, int &literalPointer, int &result);

bool getOperand(vector<Literal> &program, int &literalPointer, int &operand) {
	if (program[literalPointer].getType() == WORD_LITERAL) {
		// number or variable
		if (!getWordValue(program[literalPointer].getValue(), operand)) {
			return false;
		}
		literalPointer++;
		return true;
	} else if (program[literalPointer] == Literal("(",SIGN_LITERAL)) {
		// calculating expression in brackets to just number operand
		literalPointer++; // opening '('
		int expressionValue;
		if (!calculateExpression(program, literalPointer, expressionValue)) {
			return false;
		}
		if (program[literalPointer] != Literal(")", SIGN_LITERAL)) {
			cout << "Not complited patherness sequence" << endl;
			return false;
		}
		literalPointer++; // closing ')'
		operand = expressionValue;
		return true;
	}
	cout << "Wrong operand: " << program[literalPointer].getValue() << endl;
	return false;
}

bool getOperator(vector<Literal> &program, int &literalPointer, char &math_operator) {
	if (program[literalPointer].getType() == SIGN_LITERAL){
		string sign = program[literalPointer].getValue();
		if (sign == "+" || sign == "-" || sign == "*" || sign == "/"
		|| sign == "%") {
			math_operator = sign[0];
			literalPointer++;
			return true;
		}
	}
	return false;
}

// calculates expressions in brackets, then separates simplified expression to
// operators (ints) and operands
bool parseExpression(vector<Literal> &program, int &literalPointer,
vector<int> &operands, vector<char> &operators) {
	// expression contain math signs, correct bracket sequences, numbers,
	// variables. ends by ';',')' signs and if vector ends
	for(;;){
		int operand;
		char math_operator;
		// if operand is bracket with expression inside, it calculates and
		// result is used as operand
		if (!getOperand(program, literalPointer, operand)) {
			return false;
		}
		if (!getOperator(program, literalPointer, math_operator)) {
			// check end of expression
			if (literalPointer == program.size()
			|| program[literalPointer].getValue() == ";"
			|| program[literalPointer].getValue() == ")") {
				operands.push_back(operand);
				operators.push_back(';'); // considered as lowest priority
				// empty operator
				break; // expression parsed successfully
			}
			// invalid operator
			cout << "Incorrect operator: " << program[literalPointer].getValue() << endl;
			return false;
		}
		operands.push_back(operand);
		operators.push_back(math_operator);
	}
	return true;
}

int getOperatorPriority(char math_operator) {
	switch (math_operator) {
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
	case '%':
		return 2;
	}
	return 0;
}

bool calculate(int operand1, int operand2, char math_operator, int &result) {
	switch (math_operator) {
	case '+':
	result = operand1 + operand2;
		break;
	case '-':
	result = operand1 - operand2;
		break;
	case '*':
	result = operand1 * operand2;
		break;
	case '/':
	result = operand1 / operand2;
		break;
	case '%':
	result = operand1 % operand2;
		break;
	default:
		return false;
	}
	return true;
}

// calculates value of math expression without brackets
// expression is separated to arrays of operands and operators
// saves result to corresponding argument-variable
bool twoStackAlgorithm(const vector<int> &operands,
const vector<char> &operators, int &result) {
	stack<int> operandStack;
	stack<char> operatorStack;
	// last operand and operator in stacks are separated to variables for
	// availability of its prenultimate ones
	int currentOperand = operands[0];
	char currentOperator = operators[0];

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
			char math_operator = operatorStack.top();
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
bool calculateExpression(vector<Literal> &program, int &literalPoint,
int &result) {
	// preparing expression for algorithm: separating operands from operators
	// and calculating expresions in brackets
	vector<int> operands;
	vector<char> operators;
	if (!parseExpression(program,literalPoint,operands,operators)) {
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
bool handleAssignInstruction(vector<Literal> &program, int &literalPoint) {
	string variableName = program[literalPoint].getValue();
	if (isNumber(variableName)) {
		cout << "Can not assign new value to a number: " << variableName
		<< endl;
		return false;
	}
	if ( !(doesVariableExist(variableName) || variableName == "CON") ) {
		cout << "Variable " << variableName << " was not declared" << endl;
		return false;
	}
	if (program[literalPoint + 1] != Literal("=",SIGN_LITERAL)) {
		cout << "Assignment pattern expects operator '=' after variable name"
		<< endl;
		return false;
	}
	literalPoint += 2; // "name ="
	int result;
	if (!calculateExpression(program, literalPoint, result)) {
		return false;
	}
	if (program[literalPoint] != Literal(";",SIGN_LITERAL)) {
		cout << "Template define expects ';' at the end" << endl;
		return false;
	}
	literalPoint += 1; // ";"
	if (variableName == "CON") { // reserved name for console working
		cout << "CON: " << result << endl;
		return true;
	}
	assignVariable(variableName, result); // usual variable assignment
	return true;
}

// handles variable declaration instruction
bool handleVarInstruction(vector<Literal> &program, int &literalPointer) {
	if (program[literalPointer+1].getType() != WORD_LITERAL) {
		cout << "Template var meets incorrect format of variable name" << endl;
		return false;
	}
	if (program[literalPointer+2] != Literal(";",SIGN_LITERAL)) {
		cout << "Template var expects ';' at the end" << endl;
		return false;
	}
	string variableName = program[literalPointer+1].getValue();
	declareVariable(variableName);
	literalPointer += 3; // WORD(var) WORD(variableName) SIGN(;)
	return true;
}

// handles variable declaration deletion instruction
bool handleDeleteInstruction(vector<Literal> &program, int &literalPointer) {
	if (program[literalPointer+1].getType() != WORD_LITERAL) {
		cout << "Template delete meets incorrect format of variable name"
		<< endl;
		return false;
	}
	if (program[literalPointer+2] != Literal(";",SIGN_LITERAL)) {
		cout << "Template delete expects ';' at the end" << endl;
		return false;
	}
	string variableName = program[literalPointer+1].getValue();
	if (!doesVariableExist(variableName)) {
		cout << "Deletion of not existing variable "
		<< variableName << endl;
		return false;
	}
	deleteVariable(variableName);
	literalPointer += 3; // WORD(delete) WORD(variableName) SIGN(;)
	return true;
}

// starts program execution from literalPointer
// returns true if success
bool executeBlock(vector<Literal> &program, int &literalPointer) {
	while (literalPointer < program.size()
	&& program[literalPointer] != Literal("}", SIGN_LITERAL)) {
		if (program[literalPointer] == Literal("{", SIGN_LITERAL)) {
			// {...}
			literalPointer++; // opening '}'
			if (!executeBlock(program, literalPointer)) {
				return false;
			}
			if (program[literalPointer] != Literal("}", SIGN_LITERAL)) {
				cout << "Block end character '}' expected" << endl;
				return false;
			}
			literalPointer++; // closing '}'
		} else 	if (program[literalPointer] == Literal("var",WORD_LITERAL)) {
			// var name;
			if (!handleVarInstruction(program, literalPointer)) {
				return false;
			}
		} else 	if (program[literalPointer] == Literal("delete",WORD_LITERAL)) {
			// delete name;
			if (!handleDeleteInstruction(program, literalPointer)) {
				return false;
			}
		} else if (program[literalPointer].getType() != WORD_LITERAL) {
			cout << "Unexpected non-word literal: "
			<< program[literalPointer].getValue() << endl;
			return false;
		} else {
			// name=<expression>;
			if (!handleAssignInstruction(program, literalPointer)) {
				return false;
			}
		}
	}
	return true;
}

// it's just shell for executeBlock function to execute whole program as block
bool execute(vector<Literal> &program) {
	int literalPointer = 0;
	return executeBlock(program, literalPointer);
}
