#pragma once
#include "instruction.h"

class Expression : public Instruction{
public:
    Expression();
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
