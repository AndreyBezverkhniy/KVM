#pragma once
#include <exception>
#include "lexeme.h"

using namespace std;

class CompileTimeErrorException:exception{
    string message;
public:
    CompileTimeErrorException();
    CompileTimeErrorException(string cause,Lexeme place);
	void SetMessage(string cause,Lexeme place);
    const char* what() const noexcept override;
};
