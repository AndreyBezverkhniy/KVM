#include <memory>
#include "utils.h"
#include "instruction.h"
#include "var_declaration.h"
#include "expression.h"
#include "bin_operator.h"
#include "left_unary_operator.h"
#include "right_unary_operator.h"
#include "number.h"
#include "variable_name.h"
#include "function_call.h"
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
	if(auto ptr=dynamic_cast<const Var*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else if(auto ptr=dynamic_cast<const Return*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else if(auto ptr=dynamic_cast<const While*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else if(auto ptr=dynamic_cast<const If*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else if(auto ptr=dynamic_cast<const Block*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else if(auto ptr=dynamic_cast<const BinOperator*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else if(auto ptr=dynamic_cast<const LeftUnaryOperator*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else 	if(auto ptr=dynamic_cast<const RightUnaryOperator*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else 	if(auto ptr=dynamic_cast<const Number*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else 	if(auto ptr=dynamic_cast<const VariableName*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	} else 	if(auto ptr=dynamic_cast<const FunctionCall*>(this)){
		if(!ptr->SaveInner(os)){
			return false;
		}
	}
	return true;
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
