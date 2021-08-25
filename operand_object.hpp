#pragma once

#include <iostream>
#include <string>

using namespace std;

struct OperandObject{
	string variableName; // name of variable or empty string
	int value;
	OperandObject();
	OperandObject(string _name, int _value);
	friend ostream& operator<<(ostream &os, OperandObject operatorObject){
		os << "<" << operatorObject.variableName << ","
		<< operatorObject.value << ">";
		return os;
	}
};
