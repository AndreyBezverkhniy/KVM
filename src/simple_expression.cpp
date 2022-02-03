#include "simple_expression.h"
#include "utils.h"

SimpleExpression::SimpleExpression(){}
bool SimpleExpression::SaveInner(ostream &os) const {
	return true;
}
bool SimpleExpression::LoadInner(istream &is){
	return true;
}
