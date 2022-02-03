#include "operand.h"
#include "utils.h"
#include "element.h"

string Operand::GetOperandType() const {
	if(dynamic_cast<const Element*>(this)){
		return ELEMENT_TYPE;
	}
	return "operand_type_error";
}
bool Operand::SaveInner(ostream &os) const {
	return false;
}
bool Operand::Save(ostream &os) const {
	if(!USave(os,GetOperandType())){
		return false;
	}
	if(!SaveInner(os)){
		return false;
	}
	return true;
}
bool Operand::Load(istream &is,shared_ptr<Operand> &operand_ptr){
	string operandType;
	if(!ULoad(is,operandType)){
		return false;
	}
	bool success=true;
	if(operandType==ELEMENT_TYPE){
		shared_ptr<Element> element_ptr=make_shared<Element>();
		success=element_ptr->LoadInner(is);
		operand_ptr=element_ptr;
	} else {
		success=false;
	}
	return success;
}
