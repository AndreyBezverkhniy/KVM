#pragma once
#include "instruction.h"
#include "expression.h"
#include <memory>

class Return : public Instruction{
public:
    shared_ptr<Expression> expression;
	Return();
    void SetExpression(shared_ptr<Expression> expression);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
