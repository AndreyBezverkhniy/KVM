#pragma once
#include <exception>
#include "lexeme.h"

using namespace std;

class RunTimeErrorException:exception{
    string message;
public:
    RunTimeErrorException();
    RunTimeErrorException(string message);
	void SetMessage(string message);
    const char* what() const noexcept override;
};
