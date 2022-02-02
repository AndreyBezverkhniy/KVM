#include "bin_operator.h"
#include "utils.h"

BinOperator::BinOperator(){}
void BinOperator::SetLeft(shared_ptr<Operand> left){
	this->left=left;
}
void BinOperator::SetRight(shared_ptr<Operand> right){
	this->right=right;
}
void BinOperator::SetOperation(string operation){
	this->operation=operation;
}
bool BinOperator::SaveInner(ostream &os) const {
	if(!left || !right){
		return false;
	}
	if(!USave(os,operation)){
		return false;
	}
	if(!left->SaveInner(os)){
		return false;
	}
	if(!right->SaveInner(os)){
		return false;
	}
	return true;
}
bool BinOperator::LoadInner(istream &is){
	left=make_shared<Operand>();
	right=make_shared<Operand>();
	if(!ULoad(is,operation)){
		return false;
	}
	if(!left->LoadInner(is)){
		return false;
	}
	if(!right->LoadInner(is)){
		return false;
	}
	return true;
}
