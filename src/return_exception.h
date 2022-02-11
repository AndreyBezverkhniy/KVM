#pragma once
#include <exception>

using namespace std;

class ReturnException : exception {
	int value;
public:
	ReturnException();
	ReturnException(int value);
	const char * what () const throw ();
	int GetValue() const;
	void SetValue(int value);
};
