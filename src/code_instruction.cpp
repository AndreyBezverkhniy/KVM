#include "code_instruction.h"


bool ReadInstruction(const vector<Literal> &vec,int &index,shared_ptr<Instruction> &instruction){
    return false;
}
bool ReadBlock(const vector<Literal> &vec,int &index,shared_ptr<Block> &block){
    int new_index=index;
    if(vec[new_index++]!="{"){
        return false;
    }
    block=make_shared<Block>();
    shared_ptr<Instruction> instruction;
    while(ReadInstruction(vec,new_index,instruction)){
        block->instructions.push_back(instruction);
    }
    if(vec[new_index++]!="}"){
        return false;
    }
    index=new_index;
    return true;
}
