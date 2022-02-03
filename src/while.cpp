#include "while.h"
#include "utils.h"

While::While(){}
void While::SetCondition(shared_ptr<Expression> condition){
	this->condition=condition;
}
void While::SetInstruction(shared_ptr<Instruction> instruction){
	this->instruction=instruction;
}
bool While::SaveInner(ostream &os) const {
	if(!condition||!instruction){
		return false;
	}
	if(!condition->Save(os)){
		return false;
	}
	if(!instruction->Save(os)){
		return false;
	}
	return true;
}
bool While::LoadInner(istream &is){
	condition=make_shared<Expression>();
	instruction=make_shared<Instruction>();
	if(!Expression::Load(is,condition)){
		return false;
	}
	if(!Instruction::Load(is,instruction)){
		return false;
	}
	return true;
}
