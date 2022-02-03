#pragma once
#include "instruction.h"
#include "expression.h"
#include <memory>

class If : public Instruction{
public:
    shared_ptr<Expression> condition;
    shared_ptr<Instruction> instructionTrue;
	shared_ptr<Instruction> instructionFalse;
	If();
    void SetCondition(shared_ptr<Expression> expression);
    void SetTrueInstruction(shared_ptr<Instruction> instructionTrue);
    void SetFalseInstruction(shared_ptr<Instruction> instructionFalse);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
