#include <memory>
#include "utils.h"
#include "instruction.h"
#include "var_declaration.h"
#include "expression.h"
#include "return.h"
#include "while.h"
#include "if.h"
#include "block.h"

string Instruction::GetType() const {
	if(dynamic_cast<const Var*>(this)){
		return VAR_TYPE;
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
	if(dynamic_cast<const Block*>(this)){
		return BLOCK_TYPE;
	}
	return "instruction_type_error";
}
bool Instruction::IsInstruction(string type){
	return type==VAR_TYPE || type==RETURN_TYPE || type==WHILE_TYPE ||
	type==IF_TYPE || type==BLOCK_TYPE || Expression::IsExpression(type);
}
bool Instruction::Save(ostream &os) const {
	if(!USave(os,GetType())){
		return false;
	}
	return SaveInner(os);
}
bool Instruction::SaveInner(ostream &os) const {
	return false;
}
bool Instruction::Load(istream &is,shared_ptr<Instruction> &instruction_ptr){
	string type;
	if(!ULoad(is,type)){
		return false;
	}
	return LoadTyped(type,is,instruction_ptr);
}
bool Instruction::LoadTyped(string type,istream &is,shared_ptr<Instruction> &instruction_ptr){
	if(type==VAR_TYPE){
		return TypedLoadInner<Var,Instruction>(is,instruction_ptr);
	} else if(type==RETURN_TYPE){
		return TypedLoadInner<Return,Instruction>(is,instruction_ptr);
	} else if(type==WHILE_TYPE){
		return TypedLoadInner<While,Instruction>(is,instruction_ptr);
	} else if(type==IF_TYPE){
		return TypedLoadInner<If,Instruction>(is,instruction_ptr);
	} else if(type==BLOCK_TYPE){
		return TypedLoadInner<Block,Instruction>(is,instruction_ptr);
	} else if(Expression::IsExpression(type)){
		shared_ptr<Expression> expression_ptr=make_shared<Expression>();
		bool success=Expression::LoadTyped(type,is,expression_ptr);
		instruction_ptr=expression_ptr;
		return success;
	}
	return false;
}
