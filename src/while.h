#pragma once
#include "instruction.h"
#include "expression.h"
#include <memory>

class While : public Instruction{
public:
    shared_ptr<Expression> condition;
    shared_ptr<Instruction> instruction;
	While();
    void SetCondition(shared_ptr<Expression> expression);
    void SetInstruction(shared_ptr<Instruction> instruction);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
