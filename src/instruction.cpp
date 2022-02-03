#include <memory>
#include "instruction.h"
#include "var_declaration.h"
#include "expression.h"
#include "return.h"
#include "utils.h"

string Instruction::GetInstructionType() const {
	if(dynamic_cast<const Var*>(this)){
		return VAR_TYPE;
	}
	if(dynamic_cast<const Expression*>(this)){
		return EXPRESSION_TYPE;
	}
	if(dynamic_cast<const Return*>(this)){
		return RETURN_TYPE;
	}
	return "instruction_type_error";
}
bool Instruction::SaveInner(ostream &os) const {
	return false;
}
bool Instruction::Save(ostream &os) const {
	if(!USave(os,GetInstructionType())){
		return false;
	}
	if(auto ptr=dynamic_cast<const Var*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else if(auto ptr=dynamic_cast<const Expression*>(this)){
		if(!ptr->Save(os)){
			return false;
		}
	} else 	if(auto ptr=dynamic_cast<const Return*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
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
	} else if(instructionType==EXPRESSION_TYPE){
		shared_ptr<Expression> expression_ptr=make_shared<Expression>();
		success=Expression::Load(is,expression_ptr);
		instruction_ptr=expression_ptr;
	} else if(instructionType==RETURN_TYPE){
		shared_ptr<Return> return_ptr=make_shared<Return>();
		success=return_ptr->LoadInner(is);
		instruction_ptr=return_ptr;
	} else {
		success=false;
	}
	return success;
}
