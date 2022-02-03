#include "operand.h"
#include "utils.h"
#include "element.h"
#include "left_unary_operator.h"
#include "right_unary_operator.h"

string Operand::GetOperandType() const {
	if(dynamic_cast<const Element*>(this)){
		return ELEMENT_TYPE;
	}
	if(dynamic_cast<const LeftUnaryOperator*>(this)){
		return LEFT_UNARY_OPERATOR_TYPE;
	}
	if(dynamic_cast<const RightUnaryOperator*>(this)){
		return RIGHT_UNARY_OPERATOR_TYPE;
	}
	return "operand_type_error";
}
bool Operand::SaveInner(ostream &os) const {
	return false;
}
bool Operand::Save(ostream &os) const {
	if(!USave(os,GetOperandType())){
		return false;
	}
	if(!SaveInner(os)){
		return false;
	}
	return true;
}
bool Operand::Load(istream &is,shared_ptr<Operand> &operand_ptr){
	string operandType;
	if(!ULoad(is,operandType)){
		return false;
	}
	bool success=true;
	if(operandType==ELEMENT_TYPE){
		shared_ptr<Element> element_ptr=make_shared<Element>();
		success=element_ptr->LoadInner(is);
		operand_ptr=element_ptr;
	} else	if(operandType==LEFT_UNARY_OPERATOR_TYPE){
		shared_ptr<LeftUnaryOperator> left_unary_operator_ptr=make_shared<LeftUnaryOperator>();
		success=left_unary_operator_ptr->LoadInner(is);
		operand_ptr=left_unary_operator_ptr;
	} else	if(operandType==RIGHT_UNARY_OPERATOR_TYPE){
		shared_ptr<RightUnaryOperator> right_unary_operator=make_shared<RightUnaryOperator>();
		success=right_unary_operator->LoadInner(is);
		operand_ptr=right_unary_operator;
	} else {
		success=false;
	}
	return success;
}
