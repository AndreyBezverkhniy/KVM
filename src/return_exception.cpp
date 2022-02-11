#include "return_exception.h"

ReturnException::ReturnException(){}
ReturnException::ReturnException(int value){
	SetValue(value);
}
const char * ReturnException::what () const throw () {
	return "Return Exception";
}
int ReturnException::GetValue() const{
	return value;
}
void ReturnException::SetValue(int value){
	this->value=value;
}

