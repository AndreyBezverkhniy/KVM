#include "code_instruction.h"
#include "code_expression.h"
#include "utils.h"

bool ReadInstruction(const vector<Literal> &vec,int &index,shared_ptr<Instruction> &instruction){
    shared_ptr<Block> block;
    if(ReadBlock(vec,index,block)){
        instruction=block;
        return true;
    }
    shared_ptr<Var> var;
    if(ReadVar(vec,index,var)){
        instruction=var;
        return true;
    }
    shared_ptr<Expression> expression;
    if(ReadExpressionInstruction(vec,index,expression)){
        instruction=expression;
        return true;
    }
    return false;
}
bool ReadBlock(const vector<Literal> &vec,int &index,shared_ptr<Block> &block){
    int new_index=index;
    if(vec[new_index++]!="{"){
        return false;
    }
    shared_ptr<Block> ptr=make_shared<Block>();
    shared_ptr<Instruction> instruction;
    while(ReadInstruction(vec,new_index,instruction)){
        ptr->instructions.push_back(instruction);
    }
    if(vec[new_index++]!="}"){
        return false;
    }
    index=new_index;
    block=ptr;
    return true;
}
bool ReadVar(const vector<Literal> &vec,int &index,shared_ptr<Var> &var){
	int new_index=index;
	if(vec[new_index++]!="var"){
		return false;
	}
    var=make_shared<Var>();
	string identifier;
	while(IsIdentifier(identifier=vec[new_index].str)){
		new_index++;
        shared_ptr<Expression> expression=make_shared<Number>(123);
        if(vec[new_index]=="="){
            new_index++;
            if(!ReadExpression(vec,new_index,expression)){
                return false;
            }
        }
        pair<string,shared_ptr<Expression>> pair(
            identifier,
            expression
        );
		var->declarations.push_back(pair);
		if(vec[new_index]!=","){
			break;
		}
		new_index++;
	}
	if(vec[new_index++]!=";"){
		return false;
	}
	index=new_index;
	return true;
}
bool ReadExpressionInstruction(const vector<Literal> &vec,int &index,shared_ptr<Expression> &expression){
    int new_index=index;
    if(!ReadExpression(vec,new_index,expression)){
        return false;
    }
    if(vec[new_index++].str!=";"){
        return false;
    }
    index=new_index;
    return true;
}
