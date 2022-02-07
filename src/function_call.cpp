#include "function_call.h"
#include "utils.h"

FunctionCall::FunctionCall(){}
void FunctionCall::SetFunctionSignature(FunctionSignature signature){
	this->signature=signature;
}
bool FunctionCall::SaveInner(ostream &os) const {
	if(!USave(os,signature.func_name)){
		return false;
	}
	int size=signature.arguments.size();
	if(!USave(os,size)){
		return false;
	}
    for(int i=0;i<size;i++){
        if(!arguments[i]->Save(os)){
            return false;
        }
    }
	return true;
}
bool FunctionCall::LoadInner(istream &is){
	if(!ULoad(is,signature.func_name)){
		return false;
	}
	int size=signature.arguments.size();
	if(!ULoad(is,size)){
		return false;
	}
	signature.arguments.resize(size);
	shared_ptr<Expression> expression;
	for(int i=0;i<size;i++){
		if(!Expression::Load(is,expression)){
			return false;
		}
		arguments.push_back(expression);
	}
	return true;
}
