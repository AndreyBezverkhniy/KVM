#include <memory>
#include "instruction.h"
#include "var_declaration.h"
#include "utils.h"

string Instruction::GetInstructionType() const {
	if(dynamic_cast<const Var*>(this)){
		return VAR_TYPE;
	}
	return "instruction_type_error";
}
bool Instruction::SaveInner(ostream &os) const {
	return false;
}
bool Instruction::Save(ostream &os) const {
	if(!USave(os,VAR_TYPE)){//GetInstructionType())){
		return false;
	}
	if(!SaveInner(os)){
		return false;
	}
	return true;
}
bool Instruction::Load(istream &is,shared_ptr<Instruction> &instruction_ptr){
	string instructionType;
	if(!ULoad(is,instructionType)){
		return false;
	}
	bool success=true;
	if(instructionType==VAR_TYPE){
		shared_ptr<Var> var_ptr=make_shared<Var>();
		success=var_ptr->LoadInner(is);
		instruction_ptr=var_ptr;
	} else {
		success=false;
	}
	return success;
}
