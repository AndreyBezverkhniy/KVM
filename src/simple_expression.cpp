#include "simple_expression.h"
#include "utils.h"
#include "number.h"

string SimpleExpression::GetSimpleExpressionType() const {
	if(dynamic_cast<const Number*>(this)){
		return NUMBER_TYPE;
	}
	return "simple_expression_type_error";
}
bool SimpleExpression::SaveInner(ostream &os) const {
	return false;
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
	} else {
		success=false;
	}
	return success;
}
