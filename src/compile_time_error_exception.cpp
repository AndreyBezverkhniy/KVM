#include "compile_time_error_exception.h"

CompileTimeErrorException::CompileTimeErrorException(){}
CompileTimeErrorException::CompileTimeErrorException(string cause,Lexeme place=Lexeme()){
	SetMessage(cause,place);
}
void CompileTimeErrorException::SetMessage(string cause,Lexeme place=Lexeme()){
	if(place!=Lexeme()){
		message=place.GetFile()+" ("+to_string(place.GetLine())+":"
		+to_string(place.GetColumn())+"): ";
	}
	message+=cause;
}
const char* CompileTimeErrorException::what() const noexcept {
	return message.c_str();
}
