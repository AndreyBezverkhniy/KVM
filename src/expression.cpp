#include <memory>
#include "expression.h"
#include "operand.h"
#include "bin_operator.h"
#include "utils.h"

string Expression::GetExpressionType() const {
	if(dynamic_cast<const Operand*>(this)){
		return OPERAND_TYPE;
	}
	if(dynamic_cast<const BinOperator*>(this)){
		return BIN_OPERATOR_TYPE;
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
	if(auto ptr=dynamic_cast<const Operand*>(this)){
		if(!ptr->Save(os)){
			return false;
		}
	} else 	if(auto ptr=dynamic_cast<const BinOperator*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
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
		success=Operand::Load(is,operand_ptr);
		expression_ptr=operand_ptr;
	} else if(expressionType==BIN_OPERATOR_TYPE){
		shared_ptr<BinOperator> bin_operator_ptr=make_shared<BinOperator>();
		success=bin_operator_ptr->LoadInner(is);
		expression_ptr=bin_operator_ptr;
	} else {
		success=false;
	}
	return success;
}
