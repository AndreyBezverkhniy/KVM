#include "unary_operator.h"
#include "utils.h"

UnaryOperator::UnaryOperator(){}
void UnaryOperator::SetOperand(shared_ptr<Operand> operand){
	this->operand=operand;
}
void UnaryOperator::SetOperation(string operation){
	this->operation=operation;
}
bool UnaryOperator::SaveInner(ostream &os) const {
	if(!operand){
		return false;
	}
	if(!USave(os,operation)){
		return false;
	}
	if(!operand->Save(os)){
		return false;
	}
	return true;
}
bool UnaryOperator::LoadInner(istream &is){
	operand=make_shared<Operand>();
	if(!ULoad(is,operation)){
		return false;
	}
	if(!Operand::Load(is,operand)){
		return false;
	}
	return true;
}
