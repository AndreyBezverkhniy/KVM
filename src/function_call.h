#pragma once
#include "simple_expression.h"
#include "function_signature.h"
#include <vector>

class FunctionCall : public SimpleExpression{
public:
    FunctionSignature signature;
    vector<shared_ptr<Expression>> arguments;
	FunctionCall();
    void SetFunctionSignature(FunctionSignature signature);
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
