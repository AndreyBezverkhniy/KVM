#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include "literal.hpp"
#include "program memory.hpp"
#include "program function.hpp"

using namespace std;

bool calculateExpression(vector<Literal> &program, int &literalPointer,
int &result);

bool executeBlock(vector<Literal> &program, int &literalPointer,
int &returnValue, bool &returned);

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
	if (false) {
		//just to align elseif
	} else if (math_operator == "||") {
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

bool getOperator(vector<Literal> &program, int &literalPointer, string &math_operator) {
	if (program[literalPointer].getType() == SIGN_LITERAL){
		string sign = program[literalPointer].getValue();
		if (getOperatorPriority(sign) != 0) {
			math_operator = sign;
			literalPointer++;
			return true;
		}
	}
	return false;
}

// calculates expressions in brackets, then separates simplified expression to
// operators (ints) and operands
bool parseExpression(vector<Literal> &program, int &literalPointer,
vector<int> &operands, vector<string> &operators) {
	// expression contain math signs, correct bracket sequences, numbers,
	// variables. ends by ';', ')', ',' signs and if vector ends
	for(;;){
		int operand;
		string math_operator;
		// if operand is bracket with expression inside, it calculates and
		// result is used as operand
		if (!getOperand(program, literalPointer, operand)) {
			return false;
		}
		if (!getOperator(program, literalPointer, math_operator)) {
			// check end of expression
			if (literalPointer == program.size()
			|| program[literalPointer].getValue() == ";"
			|| program[literalPointer].getValue() == ")"
			|| program[literalPointer].getValue() == ",") {
				operands.push_back(operand);
				operators.push_back(";"); // considered as lowest priority
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
bool calculateExpression(vector<Literal> &program, int &literalPointer,
int &result) {
	// preparing expression for algorithm: separating operands from operators
	// and calculating expresions in brackets
	vector<int> operands;
	vector<string> operators;
	if (!parseExpression(program,literalPointer,operands,operators)) {
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
bool handleAssignInstruction(vector<Literal> &program, int &literalPointer) {
	string variableName = program[literalPointer].getValue();
	if (isNumber(variableName)) {
		cout << "Can not assign new value to a number: " << variableName
		<< endl;
		return false;
	}
	if ( !(doesVariableExist(variableName) || variableName == "CON") ) {
		cout << "Variable " << variableName << " was not declared" << endl;
		return false;
	}
	if (program[literalPointer + 1] != Literal("=",SIGN_LITERAL)) {
		cout << "Assignment pattern expects operator '=' after variable name"
		<< endl;
		return false;
	}
	literalPointer += 2; // "name ="
	int result;
	if (!calculateExpression(program, literalPointer, result)) {
		return false;
	}
	if (program[literalPointer] != Literal(";",SIGN_LITERAL)) {
		cout << "Template define expects ';' at the end" << endl;
		return false;
	}
	literalPointer += 1; // ";"
	if (variableName == "CON") { // reserved name for console working
		cout << "CON: " << result << endl;
		return true;
	}
	assignVariable(variableName, result); // usual variable assignment
	return true;
}

// handles variable declaration instruction
bool handleVarInstruction(vector<Literal> &program, int &literalPointer, vector<string> &variableNames) {
	literalPointer++; // WORD(var)
	for (;;) {
		if (program[literalPointer].getType() != WORD_LITERAL) {
			cout << "Template var meets incorrect format of variable name" << endl;
			return false;
		}
		string name = program[literalPointer].getValue();
		variableNames.push_back(name);
		declareVariable(name);
		literalPointer++; // WORD(name)
		if (program[literalPointer] == Literal("=",SIGN_LITERAL)) {
			literalPointer++; // SIGN(=)
			int value;
			calculateExpression(program, literalPointer, value);
			assignVariable(name, value);
		}
		if (program[literalPointer] == Literal(";",SIGN_LITERAL)) {
			literalPointer++; // SIGN(;)
			break;
		}
		if (program[literalPointer] != Literal(",",SIGN_LITERAL)) {
			cout << "Variable declaration pattern expects "
			<< "';' or ',' after name " << name << endl;
			return false;
		}
		literalPointer++; // SIGN(,)
	}
	return true;
}

void skipCurrentBlock(vector<Literal> &program, int &literalPointer) {
	int blockDeep = 1; // block deep level
	while (literalPointer < program.size() && blockDeep != 0) {
		if (program[literalPointer] == Literal("{", SIGN_LITERAL)) {
			blockDeep++;
		}
		if (program[literalPointer] == Literal("}", SIGN_LITERAL)) {
			blockDeep--;
			break;
		}
		literalPointer++;
	}
}

bool findFromPosition(const vector<Literal> &program, int &literalPointer, Literal literal) {
	while (literalPointer < program.size()) {
		if (program[literalPointer] == literal) {
			break;
		}
		literalPointer++;
	}
	return (literalPointer < program.size());
}

// handles if instruction
bool handleIfInstruction(vector<Literal> &program, int &literalPointer,
int &condition, int &returnValue, bool &returned) {
	if (program[literalPointer+1] != Literal("(",SIGN_LITERAL)) {
		cout << "Expects '(' after if/while" << endl;
		return false;
	}
	literalPointer += 2; // WORD(if) SIGN('(')
	if (!calculateExpression(program,literalPointer,condition)) {
		return false;
	}
	if (program[literalPointer] != Literal(")",SIGN_LITERAL)) {
		cout << "Expects ')' after if/while(condition" << endl;
		return false;
	}
	if (program[literalPointer+1] != Literal("{",SIGN_LITERAL)) {
		cout << "Expects '{' after if/while(condition)" << endl;
		return false;
	}
	literalPointer += 2; // SIGN(')') SIGN('{')
	if (condition) {
		if (!executeBlock(program, literalPointer, returnValue, returned)) {
			return false;
		}
	} else {
		skipCurrentBlock(program, literalPointer);
	}
	if (program[literalPointer] != Literal("}",SIGN_LITERAL)) {
		cout << "Expects '}' after block end" << endl;
		return false;
	}
	literalPointer ++; // SIGN('}')
	return true;
}

// starts program execution from literalPointer
// returns true if success
bool executeBlock(vector<Literal> &program, int &literalPointer, 
int &returnValue, bool &returned) {
	returned = false;
	set<string> scopeVariables; // variables created in block witch scope
	// of the block
	while (literalPointer < program.size()
	&& program[literalPointer] != Literal("}", SIGN_LITERAL)) {
		if (program[literalPointer] == Literal("{", SIGN_LITERAL)) {
			// {...}
			literalPointer++; // opening '}'
			if (!executeBlock(program, literalPointer, returnValue,
			returned)) {
				return false;
			}
			if (program[literalPointer] != Literal("}", SIGN_LITERAL)) {
				cout << "Block end character '}' expected" << endl;
				return false;
			}
			literalPointer++; // closing '}'
			if (returned) {
				skipCurrentBlock(program, literalPointer);
			}
		} else 	if (program[literalPointer] == Literal("var",WORD_LITERAL)) {
			// var name;
			vector<string> variableNames;
			if (!handleVarInstruction(program, literalPointer, variableNames)) {
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
		} else 	if (program[literalPointer] == Literal("return",WORD_LITERAL)) {
			// return <expression>;
			literalPointer++; // return
			if (!calculateExpression(program, literalPointer, returnValue)) {
				return false;
			}
			if (program[literalPointer] != Literal(";", SIGN_LITERAL)) {
				cout << "Expects ';' after return <expression>" << endl;
				return false;
			}
			literalPointer++; // ';'
			skipCurrentBlock(program,literalPointer);
			returned = true;
		} else 	if (program[literalPointer] == Literal("function",WORD_LITERAL)) {
			// function fname(<arguments>) {...}
			if (!findFromPosition(program, literalPointer,
			Literal("{",SIGN_LITERAL))) {
				return false;
			}
			literalPointer++; // '{'
			skipCurrentBlock(program, literalPointer);
			if (program[literalPointer] != Literal("}",SIGN_LITERAL)) {
				cout << "Function body expect '}' at the end" << endl;
				return false;
			}
			literalPointer++; // '}'
		} else 	if (program[literalPointer] == Literal("if",WORD_LITERAL)) {
			// if (<expression>) {...}
			int condition;
			if (!handleIfInstruction(program, literalPointer, condition,
			returnValue, returned)) {
				return false;
			}
			if (returned) {
				skipCurrentBlock(program, literalPointer);
			}
		} else 	if (program[literalPointer] == Literal("while",WORD_LITERAL)) {
			// while (<expression>) {...}
			int condition;
			int circlePointer = literalPointer;
			do {
				literalPointer = circlePointer;
				if (!handleIfInstruction(program, literalPointer, condition,
				returnValue, returned)) {
					return false;
				}
				if (returned) {
					skipCurrentBlock(program, literalPointer);
					break;
				}
			} while (condition);
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
	for (auto variable : scopeVariables) {
		deleteVariable(variable);
	}
	return true;
}

// it's just shell for executeBlock function to execute whole program as block
bool execute(vector<Literal> &program,  int &returnValue, bool &returned) {
	int literalPointer = 0;
	return executeBlock(program, literalPointer, returnValue, returned);
}
