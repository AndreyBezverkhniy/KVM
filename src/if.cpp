#include "if.h"
#include "utils.h"

If::If(){}
void If::SetCondition(shared_ptr<Expression> condition){
	this->condition=condition;
}
void If::SetTrueInstruction(shared_ptr<Instruction> instructionTrue){
	this->instructionTrue=instructionTrue;
}
void If::SetFalseInstruction(shared_ptr<Instruction> instructionFalse){
	this->instructionFalse=instructionFalse;
}
bool If::SaveInner(ostream &os) const {
	if(!condition||!instructionTrue){
		return false;
	}
	if(!condition->Save(os)){
		return false;
	}
	if(!instructionTrue->Save(os)){
		return false;
	}
	bool if_else=(instructionFalse.get()!=nullptr);
	if(!USave(os,if_else)){
		return false;
	}
	if(if_else){
		if(!instructionFalse->Save(os)){
			return false;
		}
	}
	return true;
}
bool If::LoadInner(istream &is){
	condition=make_shared<Expression>();
	instructionTrue=make_shared<Instruction>();
	if(!Expression::Load(is,condition)){
		return false;
	}
	if(!Instruction::Load(is,instructionTrue)){
		return false;
	}
	bool if_else;
	if(!ULoad(is,if_else)){
		return false;
	}
	if(if_else){
		instructionFalse=make_shared<Instruction>();
		if(!Instruction::Load(is,instructionFalse)){
			return false;
		}
	}
	return true;
}
