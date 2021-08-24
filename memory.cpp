/*
variables:
every variable name is key to map variables, where stacks of values are stored.
creating variable with existing name pushes new int(0) field to stack assigned
with key of variable name,
i.e. "push name;" works like variables[name].push(int(0));
deleting variable pops highest value from stack.
current field connected with name is always the highest one in corresponding
stack: "name=<expression>;" works like variables[name].top()=<expression>;
value of variable is getting in the same way: variables[name].top().
//*/

#include "memory.hpp"

#include <map>
#include <string>
#include <stack>
#include "function.hpp"
#include "literal.hpp"

using namespace std;

vector<Literal> program;
map<string,FunctionDescription> functions;
map<string,stack<int>> variables;

bool doesFunctionExist(string functionName) {
	// was it declared
	return functions.find(functionName) != functions.end();
}

bool doesVariableExist(string variableName) {
	// was it declared at least one time?
	return variables[variableName].size() > 0;
}

// declare one variable declaration
void declareVariable(string variableName) {
	variables[variableName].push(0);
}

// declare multiple variable declaration
void declareVariables(vector<string> &variableNames) {
	for (string variableName : variableNames) {
		declareVariable(variableName);
	}
}

// deletes one variable declaration
void deleteVariable(string variableName) {
	variables[variableName].pop();
}

// deletes multiple variable declaration
void deleteVariables(vector<string> &variableNames) {
	for (string variableName : variableNames) {
		deleteVariable(variableName);
	}
}

// handle one assignment
void assignVariable(string variableName, int newValue) {
	int &variable = variables[variableName].top();
	variable = newValue;
}

// handle multiple assignments
void assignVariables(vector<string> &variableNames, vector<int> &newValues) {
	for (int i=0; i < variableNames.size(); i++) {
		string &variableName = variableNames[i];
		int &newValue = newValues[i];
		assignVariable(variableName, newValue);
	}
}

int getVariableValue(string variableName) {
	return variables[variableName].top();
}
