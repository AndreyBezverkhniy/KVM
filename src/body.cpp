#include "body.h"
#include "utils.h"

Body::Body(){}
bool Body::Save(ostream &os) const {
	int size=(int)instructions.size();
	if(!USave(os,size)){
		return false;
	}
	for(int i=0;i<size;i++){
		if(!instructions[i].Save(os)){
			return false;
		}
	}
	return true;
}
bool Body::Load(istream &is){
	int size;
	if(!ULoad(is,size)){
		return false;
	}
	Instruction instruction;
	for(int i=0;i<size;i++){
		if(!instruction.Load(is)){
			return false;
		}
		instructions.push_back(instruction);
	}
	return true;
}
