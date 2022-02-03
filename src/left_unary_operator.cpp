#include "left_unary_operator.h"
#include "utils.h"

LeftUnaryOperator::LeftUnaryOperator(){}
void LeftUnaryOperator::SetOperand(shared_ptr<Operand> operand){
	this->operand=operand;
}
void LeftUnaryOperator::SetOperation(string operation){
	this->operation=operation;
}
bool LeftUnaryOperator::SaveInner(ostream &os) const {
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
bool LeftUnaryOperator::LoadInner(istream &is){
	operand=make_shared<Operand>();
	if(!ULoad(is,operation)){
		return false;
	}
	if(!Operand::Load(is,operand)){
		return false;
	}
	return true;
}
