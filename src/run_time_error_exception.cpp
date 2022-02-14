#include "run_time_error_exception.h"

RunTimeErrorException::RunTimeErrorException(){}
RunTimeErrorException::RunTimeErrorException(string message){
	SetMessage(message);
}
void RunTimeErrorException::SetMessage(string message){
	this->message=message;
}
const char* RunTimeErrorException::what() const noexcept {
	return message.c_str();
}
