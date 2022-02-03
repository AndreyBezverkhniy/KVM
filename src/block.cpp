#include "block.h"
#include "utils.h"

Block::Block(){}
bool Block::SaveInner(ostream &os) const {
	int size=(int)instructions.size();
	if(!USave(os,size)){
		return false;
	}
	for(int i=0;i<size;i++){
		if(!instructions[i]->Save(os)){
			return false;
		}
	}
	return true;
}
bool Block::LoadInner(istream &is){
	int size;
	if(!ULoad(is,size)){
		return false;
	}
	shared_ptr<Instruction> instruction;
	for(int i=0;i<size;i++){
		if(!Instruction::Load(is,instruction)){
			return false;
		}
		instructions.push_back(instruction);
	}
	return true;
}
