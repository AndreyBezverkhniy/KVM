#include "return.h"
#include "utils.h"

Return::Return(){}
void Return::SetExpression(shared_ptr<Expression> expression){
	this->expression=expression;
}
bool Return::SaveInner(ostream &os) const {
	if(!expression){
		return false;
	}
	if(!expression->Save(os)){
		return false;
	}
	return true;
}
bool Return::LoadInner(istream &is){
	expression=make_shared<Expression>();
	if(!Expression::Load(is,expression)){
		return false;
	}
	return true;
}
