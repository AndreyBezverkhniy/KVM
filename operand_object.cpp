#include "operand_object.hpp"

#include <string>

using namespace std;

OperandObject::OperandObject(){
	variableName = "";
	value = 0;
}

OperandObject::OperandObject(string _name, int _value){
	variableName = _name;
	value = _value;
}

ostream& operator<<(ostream &os, OperandObject operatorObject){
	os << "<" << operatorObject.variableName << ","
	<< operatorObject.value << ">";
	return os;
}
