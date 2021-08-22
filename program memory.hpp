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

#pragma once

#include <map>
#include <string>
#include <stack>

using namespace std;

map<string,stack<int>> variables;

bool doesVariableExist(string variableName) {
	// was it declared at least one time?
	return variables[variableName].size() > 0;
}

void declareVariable(string variableName) {
	variables[variableName].push(0);
}

// deletes one variable declaration
void deleteVariable(string variableName) {
	variables[variableName].pop();
}

void assignVariable(string variableName, int newValue) {
	int &variable = variables[variableName].top();
	variable = newValue;
}

int getVariableValue(string variableName) {
	return variables[variableName].top();
}
