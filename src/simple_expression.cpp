#include "simple_expression.h"
#include "utils.h"
#include "number.h"
#include "variable_name.h"
#include "function_call.h"

string SimpleExpression::GetType() const {
	if(dynamic_cast<const Number*>(this)){
		return NUMBER_TYPE;
	}
	if(dynamic_cast<const VariableName*>(this)){
		return VARIABLE_NAME_TYPE;
	}
	if(dynamic_cast<const FunctionCall*>(this)){
		return FUNCTION_CALL_TYPE;
	}
	return "simple_expression_type_error";
}
bool SimpleExpression::IsSimpleExpression(string type){
	return type==NUMBER_TYPE || type==VARIABLE_NAME_TYPE || type==FUNCTION_CALL_TYPE;
}
bool SimpleExpression::Load(istream &is,shared_ptr<SimpleExpression> &simple_expression_ptr){
	string type;
	if(!ULoad(is,type)){
		return false;
	}
	return LoadTyped(type,is,simple_expression_ptr);
}
bool SimpleExpression::LoadTyped(string type,istream &is,shared_ptr<SimpleExpression> &simple_expression_ptr){
	if(type==NUMBER_TYPE){
		return TypedLoadInner<Number,SimpleExpression>(is,simple_expression_ptr);
	} else if(type==VARIABLE_NAME_TYPE){
		return TypedLoadInner<VariableName,SimpleExpression>(is,simple_expression_ptr);
	} else if(type==FUNCTION_CALL_TYPE){
		return TypedLoadInner<FunctionCall,SimpleExpression>(is,simple_expression_ptr);
	}
	return false;
}
