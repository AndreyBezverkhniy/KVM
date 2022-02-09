#include "function_call.h"
#include "utils.h"

FunctionCall::FunctionCall(){}
bool FunctionCall::SaveInner(ostream &os) const {
	if(!signature.Save(os)){
		return false;
	}
    for(int i=0;i<arguments.size();i++){
        if(!arguments[i]->Save(os)){
            return false;
        }
    }
	return true;
}
bool FunctionCall::LoadInner(istream &is){
	if(!signature.Load(is)){
		return false;
	}
	shared_ptr<Expression> expression;
	for(int i=0;i<signature.arg_n;i++){
		if(!Expression::Load(is,expression)){
			return false;
		}
		arguments.push_back(expression);
	}
	return true;
}
