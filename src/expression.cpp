#include <memory>
#include "expression.h"
#include "operand.h"
#include "bin_operator.h"
#include "utils.h"

string Expression::GetType() const {
	if(dynamic_cast<const BinOperator*>(this)){
		return BIN_OPERATOR_TYPE;
	}
	return "expression_type_error";
}
bool Expression::IsExpression(string type){
	return type==BIN_OPERATOR_TYPE || Operand::IsOperand(type);
}
bool Expression::Load(istream &is,shared_ptr<Expression> &expression_ptr){
	string type;
	if(!ULoad(is,type)){
		return false;
	}
	return LoadTyped(type,is,expression_ptr);
}
bool Expression::LoadTyped(string type,istream &is,shared_ptr<Expression> &expression_ptr){
	if(type==BIN_OPERATOR_TYPE){
		return TypedLoadInner<BinOperator,Expression>(is,expression_ptr);
	} else if(Operand::IsOperand(type)){
		shared_ptr<Operand> operand_ptr=make_shared<Operand>();
		bool success=Operand::LoadTyped(type,is,operand_ptr);
		expression_ptr=operand_ptr;
		return success;
	}
	return false;
}
