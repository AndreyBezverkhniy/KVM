#pragma once
#include <iostream>
#include <string>
#include <memory>
#include "typed_load_inner.h"

using namespace std;

#define VAR_TYPE "var"
#define RETURN_TYPE "return"
#define WHILE_TYPE "while"
#define IF_TYPE "if"
#define BLOCK_TYPE "block"

class Instruction{
public:
	virtual string GetType() const;
	static bool IsInstruction(string type);
	bool Save(ostream &os) const;
	virtual bool SaveInner(ostream &os) const;
	static bool Load(istream &is,shared_ptr<Instruction> &instruction_ptr);
	static bool LoadTyped(string type,istream &is,shared_ptr<Instruction> &instruction_ptr);
};
