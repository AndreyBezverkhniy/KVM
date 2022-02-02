#include <memory>
#include "expression.h"
#include "operand.h"
#include "utils.h"

string Expression::GetExpressionType() const {
	if(dynamic_cast<const Operand*>(this)){
		return OPERAND_TYPE;
	}
	return "expression_type_error";
}
bool Expression::SaveInner(ostream &os) const {
	return false;
}
bool Expression::Save(ostream &os) const {
	if(!USave(os,GetExpressionType())){
		return false;
	}
	if(!SaveInner(os)){
		return false;
	}
	return true;
}
bool Expression::Load(istream &is,shared_ptr<Expression> &expression_ptr){
	string expressionType;
	if(!ULoad(is,expressionType)){
		return false;
	}
	bool success=true;
	if(expressionType==OPERAND_TYPE){
		shared_ptr<Operand> operand_ptr=make_shared<Operand>();
		success=operand_ptr->LoadInner(is);
		expression_ptr=operand_ptr;
	} else {
		success=false;
	}
	return success;
}
