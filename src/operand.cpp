#include "operand.h"
#include "utils.h"

Operand::Operand(){}
bool Operand::SaveInner(ostream &os) const {
	return true;
}
bool Operand::LoadInner(istream &is){
	return true;
}
