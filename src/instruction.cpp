#include <memory>
#include "utils.h"
#include "instruction.h"
#include "var_declaration.h"
#include "expression.h"
#include "return.h"
#include "while.h"
#include "if.h"

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
	if(dynamic_cast<const While*>(this)){
		return WHILE_TYPE;
	}
	if(dynamic_cast<const If*>(this)){
		return IF_TYPE;
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
	} else 	if(auto ptr=dynamic_cast<const While*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else 	if(auto ptr=dynamic_cast<const If*>(this)){
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
	} else if(instructionType==WHILE_TYPE){
		shared_ptr<While> while_ptr=make_shared<While>();
		success=while_ptr->LoadInner(is);
		instruction_ptr=while_ptr;
	} else if(instructionType==IF_TYPE){
		shared_ptr<If> if_ptr=make_shared<If>();
		success=if_ptr->LoadInner(is);
		instruction_ptr=if_ptr;
	} else {
		success=false;
	}
	return success;
}
