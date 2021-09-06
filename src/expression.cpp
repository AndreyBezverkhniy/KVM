#include "expression.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "literal.hpp"
#include "memory.hpp"
#include "function.hpp"
#include "execute_program.hpp"
#include "utils.hpp"
#include "operand_object.hpp"
#include "inbuilt_functions.hpp"

using namespace std;

bool callFunction(string functionName, const vector<int> &arguments,
OperandObject &result) {

	FunctionDescription &description = functions[functionName];

	// declare function scope variables - arguments
	if (description.arguments.size() != arguments.size()) {
		cout << "Incorrect argument amount of function "
		<< functionName << endl;
		return false;
	}
	declareVariables(description.arguments);
	assignVariables(description.arguments, arguments);

	// go to function body to execute
	int literalIntex = description.bodyIntex;

	// execute function
	result = OperandObject("", 0); // by default
	bool garbage;
	if (!executeBlock(literalIntex,result.value,garbage)) {
		return false;
	}

	// delete function scope variables - arguments
	deleteVariables(description.arguments);

	return true;

}

bool parseFunctionCall(int &literalIntex, string &functionName,  vector<int> &arguments) {

	functionName = program[literalIntex].getValue();
	literalIntex++; // functionName

	if (!parseExactLiteral(literalIntex, "(")) { // '('
		return false;
	}

	// arguments
	arguments.clear();
	int argumentNumber = 0;
	while (!parseExactLiteral(literalIntex, ")", false)) { // ')'

		if (argumentNumber > 0) {
			if (!parseExactLiteral(literalIntex, ",", false)) { // ','
				break;
			}
		}

		// argument
		int valuePassed;
		OperandObject valueObject;
		if (!calculateExpression(literalIntex, valueObject)) {
			return false;
		}
		valuePassed = valueObject.value;
		arguments.push_back(valuePassed);
		argumentNumber++;

	}

	return true;
	
}

// parse numbers, variables and function calls
// saves result to corresponding argument-variable
bool parseLiteralValue(int &literalIntex, OperandObject &result) {
	string word = program[literalIntex].getValue();
	if (isNumber(word)) {
		result = OperandObject("", toNumber(word));
		literalIntex++; // number
		return true;
	}
	if (doesFunctionExist(word)) {
		vector<int> arguments;
		string functionName;
		if (!parseFunctionCall(literalIntex, functionName, arguments)) {
			return false;
		}
		return callFunction(functionName, arguments, result);
	}
	if (doesInbuiltFunctionExist(word)) {
		vector<int> arguments;
		string functionName;
		if (!parseFunctionCall(literalIntex, functionName, arguments)) {
			return false;
		}
		return callInbuiltFunction(functionName, arguments, result);
	}
	if (!doesVariableExist(word)) {
		cout << "Variable " << word << " was not declared" << endl;
		return false;
	}
	result = OperandObject(word, getVariableValue(word));
	literalIntex++; // number, variable or function call
	return true;
}

bool isleftUnaryOperator(string math_operator) {
	return math_operator == "!" || math_operator == "+" || math_operator == "-"
	|| math_operator == "++" || math_operator == "--";
}

bool isRightUnaryOperator(string math_operator) {
	return math_operator == "++" || math_operator == "--";
}

bool applyleftUnaryOperator(OperandObject operand, string leftUnaryOperator,
OperandObject &result) {
	if (leftUnaryOperator == "!") {
		result.variableName = "";
		result.value = !operand.value;
	} else if (leftUnaryOperator == "+") {
		result.variableName = "";
		result.value = +operand.value;
	} else if (leftUnaryOperator == "-") {
		result.variableName = "";
		result.value = -operand.value;
	} else if (leftUnaryOperator == "++") {
		if (!doesVariableExist(operand.variableName)) {
			return false;
		}
		result.variableName = operand.variableName;
		result.value = getVariableValue(operand.variableName);
		result.value += 1;
		assignVariable(operand.variableName, result.value);
	} else if (leftUnaryOperator == "--") {
		if (!doesVariableExist(operand.variableName)) {
			return false;
		}
		result.variableName = operand.variableName;
		result.value = getVariableValue(operand.variableName);
		result.value -= 1;
		assignVariable(operand.variableName, result.value);
	} else {
		return false;
	}
	return true;
}

bool applyRightUnarOperator(OperandObject operand, string rightUnaryOperator,
OperandObject &result) {
	if (rightUnaryOperator == "++") {
		if (!doesVariableExist(operand.variableName)) {
			return false;
		}
		result.value = getVariableValue(operand.variableName);
		assignVariable(operand.variableName, result.value + 1);
		result.variableName = "";
	} else if (rightUnaryOperator == "--") {
		if (!doesVariableExist(operand.variableName)) {
			return false;
		}
		result.value = getVariableValue(operand.variableName);
		assignVariable(operand.variableName, result.value - 1);
		result.variableName = "";
	} else {
		return false;
	}
	return true;
}

bool parseOperand(int &literalIntex, OperandObject &result) {

	// number, variable or function call
	
	if (program[literalIntex].getType() == WORD_LITERAL) {
		if (!parseLiteralValue(literalIntex, result)) {
			return false;
		}
		return true;
	}

	// if unar operator then calculate

	string leftUnaryOperator = program[literalIntex].getValue();
	if (isleftUnaryOperator(leftUnaryOperator)) {
		literalIntex++;
		OperandObject operand;
		if (!parseOperand(literalIntex, operand)) {
			return false;
		}
		if (!applyleftUnaryOperator(operand, leftUnaryOperator, result)) {
			return false;
		}
		return true;
	}

	// value of expression in brackets becomes operand value

	if (!parseExactLiteral(literalIntex, "(")) { // '('
		return false;
	}

	// expression
	OperandObject expressionValue;
	if (!calculateExpression(literalIntex, expressionValue)) {
		return false;
	}

	if (!parseExactLiteral(literalIntex, ")")) { // ')'
		return false;
	}

	result = expressionValue;
	return true;

}

int getOperatorPriority(string math_operator) {
	if (math_operator == "=") {
		return 1;
	}
	if (math_operator == "||") {
		return 2;
	}
	if (math_operator == "&&") {
		return 3;
	}
	if (math_operator == "==" || math_operator == "!=" || math_operator == ">="
	|| math_operator == "<=" || math_operator == ">" || math_operator == "<") {
		return 4;
	}
	if (math_operator == "+" || math_operator == "-") {
		return 5;
	}
	if (math_operator == "*" || math_operator == "/" || math_operator == "%") {
		return 6;
	}
	return 0; // operator unknown
}

bool isOperator(string _operator) {
	return getOperatorPriority(_operator) != 0;
}

bool calculate(OperandObject operand1, OperandObject operand2,
string math_operator, OperandObject &result) {

	// if variable then get value
	if (doesVariableExist(operand1.variableName)) {
		operand1.value = getVariableValue(operand1.variableName);
	}
	if (doesVariableExist(operand2.variableName)) {
		operand2.value = getVariableValue(operand2.variableName);
	}

	result.variableName = ""; // dy default result is
	// stored temporary, not in variable

	if (math_operator == "||") {
		result.value = operand1.value || operand2.value;
	} else if (math_operator == "&&") {
		result.value = operand1.value && operand2.value;
	} else if (math_operator == "==") {
		result.value = operand1.value == operand2.value;
	} else if (math_operator == "!=") {
		result.value = operand1.value != operand2.value;
	} else if (math_operator == ">=") {
		result.value = operand1.value >= operand2.value;
	} else if (math_operator == "<=") {
		result.value = operand1.value <= operand2.value;
	} else if (math_operator == ">") {
		result.value = operand1.value > operand2.value;
	} else if (math_operator == "<") {
		result.value = operand1.value < operand2.value;
	} else if (math_operator == "+") {
		result.value = operand1.value + operand2.value;
	} else if (math_operator == "-") {
		result.value = operand1.value - operand2.value;
	} else if (math_operator == "*") {
		result.value = operand1.value * operand2.value;
	} else if (math_operator == "/") {
		result.value = operand1.value / operand2.value;
	} else if (math_operator == "%") {
		result.value = operand1.value % operand2.value;
	} else if (math_operator == "=") {
		result.variableName = operand1.variableName;
		if (!doesVariableExist(result.variableName)) {
			return false; // assignment to nowhere
		}
		result.value = operand2.value;
		assignVariable(result.variableName, result.value);
	} else {
		return false; // operator unknown
	}

	return true;

}

bool parseOperator(int &literalIntex, string &math_operator) {
	if (program[literalIntex].getType() == SIGN_LITERAL){
		string sign = program[literalIntex].getValue();
		if (isOperator(sign) || isRightUnaryOperator(sign)
		|| isleftUnaryOperator(sign)) {
			math_operator = sign;
			literalIntex++; // operator
			return true;
		}
	}
	return false;
}

// calculates expressions in brackets, then separates simplified expression to
// operators (ints) and operands
// expression contain math signs, correct bracket sequences, numbers,
// variables. ends by ';', ')', ',' signs and if vector ends
bool parseExpression(int &literalIntex,
vector<OperandObject> &operands, vector<string> &operators) {
	
	for(;;){

		OperandObject operand;
		string math_operator;
		// if operand is bracket with expression inside, it calculates and
		// result is used as operand
		if (!parseOperand(literalIntex, operand)) {
			return false;
		}

		for (;;) {
			if (!parseOperator(literalIntex, math_operator)) {
				// check end of expression
				if (program[literalIntex].getValue() == literalEOF
				|| program[literalIntex].getValue() == ";"
				|| program[literalIntex].getValue() == ")"
				|| program[literalIntex].getValue() == ",") {
					operands.push_back(operand);
					operators.push_back(";"); // considered as lowest priority
					// operator "end of expression", not handling operator
					return true; // expression parsed successfully
				}
				// invalid operator
				cout << "Incorrect operator: "
				<< program[literalIntex].getValue() << endl;
				return false;
			}
			if (isRightUnaryOperator(math_operator)) {
				OperandObject result;
				if (!applyRightUnarOperator(operand, math_operator, result)) {
					return false;
				}
				operand = result;
				continue;
			}
			break;
		}

		operands.push_back(operand);
		operators.push_back(math_operator);

	}

	return true;

}

// calculates value of math expression without brackets
// expression is separated to arrays of operands and operators
// saves result to corresponding argument-variable
bool twoStackAlgorithm(const vector<OperandObject> &operands,
const vector<string> &operators, OperandObject &result) {
	stack<OperandObject> operandStack;
	stack<string> operatorStack;
	// last operand and operator in stacks are separated to variables for
	// availability of it's prenultimate ones
	OperandObject currentOperand = operands[0];
	string currentOperator = operators[0];

	for (int i=1; i < operands.size(); i++) {
		// last operator - start point for
		// operator= handling in right-to-left way
		bool lastOperation = (i == operands.size() - 1);
		// put next operand and operator to stacks
		operandStack.push(currentOperand);
		operatorStack.push(currentOperator);
		currentOperand = operands[i];
		currentOperator = operators[i];
		// applying previous operators with not lower priority than the last
		// one in reverse order
		while (operatorStack.size() > 0 &&
		getOperatorPriority(operatorStack.top())
		>= getOperatorPriority(currentOperator) &&
		(operatorStack.top() == "=" && lastOperation // operator= is last
		|| operatorStack.top() != "=") ) {             // calculating one
			OperandObject operationResult;
			OperandObject operand1 = operandStack.top();
			operandStack.pop();
			OperandObject operand2 = currentOperand;
			string math_operator = operatorStack.top();
			operatorStack.pop();
			if (!calculate(operand1,operand2,math_operator,operationResult)) {
				cout << "Calculation error: " << operand1 << math_operator
				<< operand2 << endl;
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
bool calculateExpression(int &literalIntex, OperandObject &result) {
	// preparing expression for algorithm: separating operands from operators
	// and calculating expresions in brackets
	vector<OperandObject> operands;
	vector<string> operators;
	if (!parseExpression(literalIntex,operands,operators)) {
		return false;
	}
	if (operands.size() == 1) {
		// expression equals it's only operand
		result = operands[0];
		return true;
	}
	// two stack algorithm to calculate math expressions without brackets
	// brackets are calculated while parsing expression to operands and
	// operators
	return twoStackAlgorithm(operands, operators, result);
}
