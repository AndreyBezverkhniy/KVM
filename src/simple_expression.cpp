#include "simple_expression.h"
#include "utils.h"
#include "number.h"
#include "variable_name.h"
#include "function_call.h"

string SimpleExpression::GetSimpleExpressionType() const {
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
bool SimpleExpression::Save(ostream &os) const {
	if(!USave(os,GetSimpleExpressionType())){
		return false;
	}
	if(!SaveInner(os)){
		return false;
	}
	return true;
}
bool SimpleExpression::Load(istream &is,shared_ptr<SimpleExpression> &simple_expression_ptr){
	string simpleExpressionType;
	if(!ULoad(is,simpleExpressionType)){
		return false;
	}
	bool success=true;
	if(simpleExpressionType==NUMBER_TYPE){
		shared_ptr<Number> number_ptr=make_shared<Number>();
		success=number_ptr->LoadInner(is);
		simple_expression_ptr=number_ptr;
	} else if(simpleExpressionType==VARIABLE_NAME_TYPE){
		shared_ptr<VariableName> variable_name_ptr=make_shared<VariableName>();
		success=variable_name_ptr->LoadInner(is);
		simple_expression_ptr=variable_name_ptr;
	} else if(simpleExpressionType==FUNCTION_CALL_TYPE){
		shared_ptr<FunctionCall> function_call_ptr=make_shared<FunctionCall>();
		success=function_call_ptr->LoadInner(is);
		simple_expression_ptr=function_call_ptr;
	} else {
		success=false;
	}
	return success;
}
