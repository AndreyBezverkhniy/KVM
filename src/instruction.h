#pragma once
#include <iostream>
#include <string>
#include <memory>

using namespace std;

#define VAR_TYPE string("var")
#define EXPRESSION_TYPE string("expression")

class Instruction{
public:
	string GetInstructionType() const;
	bool Save(ostream &os) const;
	virtual bool SaveInner(ostream &os) const;
	static bool Load(istream &is,shared_ptr<Instruction> &instruction_ptr);
};
