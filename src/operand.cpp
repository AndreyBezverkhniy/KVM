#include "operand.h"
#include "utils.h"
#include "simple_expression.h"
#include "left_unary_operator.h"
#include "right_unary_operator.h"

string Operand::GetType() const {
	if(dynamic_cast<const LeftUnaryOperator*>(this)){
		return LEFT_UNARY_OPERATOR_TYPE;
	}
	if(dynamic_cast<const RightUnaryOperator*>(this)){
		return RIGHT_UNARY_OPERATOR_TYPE;
	}
	return "operand_type_error";
}
bool Operand::IsOperand(string type){
	return type==LEFT_UNARY_OPERATOR_TYPE || type==RIGHT_UNARY_OPERATOR_TYPE ||
	SimpleExpression::IsSimpleExpression(type);
}
bool Operand::Load(istream &is,shared_ptr<Operand> &operand_ptr){
	string type;
	if(!ULoad(is,type)){
		return false;
	}
	return LoadTyped(type,is,operand_ptr);
}
bool Operand::LoadTyped(string type,istream &is,shared_ptr<Operand> &operand_ptr){
	if(type==LEFT_UNARY_OPERATOR_TYPE){
		return TypedLoadInner<LeftUnaryOperator,Operand>(is,operand_ptr);
	} else if(type==RIGHT_UNARY_OPERATOR_TYPE){
		return TypedLoadInner<RightUnaryOperator,Operand>(is,operand_ptr);
	} else if(SimpleExpression::IsSimpleExpression(type)){
		shared_ptr<SimpleExpression> simple_expression_ptr=make_shared<SimpleExpression>();
		bool success=SimpleExpression::LoadTyped(type,is,simple_expression_ptr);
		operand_ptr=simple_expression_ptr;
		return success;
	}
	return false;
}
