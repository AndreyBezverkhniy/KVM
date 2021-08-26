#include "inbuilt_functions.hpp"

#include <iostream>
#include <vector>
#include <string>
#include "operand_object.hpp"
#include "utils.hpp"

using namespace std;

vector<string> inbuiltFunctions{string("write"),string("read")};

bool doesInbuiltFunctionExist(string functionName) {
	return hasVectorAnElement(inbuiltFunctions, functionName);
}

bool callInbuiltFunction(string functionName, const vector<int> &arguments,
OperandObject &result) {

	result = OperandObject("", 0); // by default

	if (functionName == "write") {
		cout << "OUT> ";
		for (int argument : arguments) {
			cout << argument << " ";
		}
		cout << endl;
		return true;
	}

	if (functionName == "read") {
		cout << "IN> ";
		cin >> result.value;
		return true;
	}

	return false;

}
