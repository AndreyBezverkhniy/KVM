#include "bin_operator.h"
#include "utils.h"

BinOperator::BinOperator(){}
void BinOperator::SetLeft(shared_ptr<Expression> left){
	this->left=left;
}
void BinOperator::SetRight(shared_ptr<Expression> right){
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
	if(!left->Save(os)){
		return false;
	}
	if(!right->Save(os)){
		return false;
	}
	return true;
}
bool BinOperator::LoadInner(istream &is){
	left=make_shared<Expression>();
	right=make_shared<Expression>();
	if(!ULoad(is,operation)){
		return false;
	}
	if(!Expression::Load(is,left)){
		return false;
	}
	if(!Expression::Load(is,right)){
		return false;
	}
	return true;
}
