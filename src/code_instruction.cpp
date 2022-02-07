#include "code_instruction.h"
#include "code_expression.h"
#include "utils.h"

bool ReadInstruction(const vector<Lexeme> &vec,int &index,shared_ptr<Instruction> &instruction){
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
    shared_ptr<Return> ret;
    if(ReadReturn(vec,index,ret)){
        instruction=ret;
        return true;
    }
    shared_ptr<While> whileI;
    if(ReadWhile(vec,index,whileI)){
        instruction=whileI;
        return true;
    }
    shared_ptr<If> ifI;
    if(ReadIf(vec,index,ifI)){
        instruction=ifI;
        return true;
    }
    shared_ptr<Expression> expression;
    if(ReadExpressionInstruction(vec,index,expression)){
        instruction=expression;
        return true;
    }
    return false;
}
bool ReadBlock(const vector<Lexeme> &vec,int &index,shared_ptr<Block> &block){
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
bool ReadVar(const vector<Lexeme> &vec,int &index,shared_ptr<Var> &var){
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
bool ReadExpressionInstruction(const vector<Lexeme> &vec,int &index,shared_ptr<Expression> &expression){
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
bool ReadReturn(const vector<Lexeme> &vec,int &index,shared_ptr<Return> &ret){
	int new_index=index;
	if(vec[new_index++]!="return"){
		return false;
	}
    ret=make_shared<Return>();
	shared_ptr<Expression> expression;
	if(vec[new_index]!=";"){
		if(!ReadExpression(vec,new_index,expression)){
			return false;
		}
	} else {
		expression=make_shared<Number>(0);
	}
	ret->expression=expression;
	if(vec[new_index++]!=";"){
		return false;
	}
	index=new_index;
	return true;
}
bool ReadWhile(const vector<Lexeme> &vec,int &index,shared_ptr<While> &whileI){
	int new_index=index;
	if(vec[new_index++]!="while"){
		return false;
	}
	if(vec[new_index++]!="("){
		return false;
	}
    shared_ptr<Expression> expression;
	if(!ReadExpression(vec,new_index,expression)){
		return false;
	}
	if(vec[new_index++]!=")"){
		return false;
	}
	shared_ptr<Instruction> instruction;
	if(!ReadInstruction(vec,new_index,instruction)){
		return false;
	}
	whileI=make_shared<While>();
	whileI->condition=expression;
	whileI->instruction=instruction;
	index=new_index;
	return true;
}
bool ReadIf(const vector<Lexeme> &vec,int &index,shared_ptr<If> &ifI){
	int new_index=index;
	if(vec[new_index++]!="if"){
		return false;
	}
	if(vec[new_index++]!="("){
		return false;
	}
    shared_ptr<Expression> expression;
	if(!ReadExpression(vec,new_index,expression)){
		return false;
	}
	if(vec[new_index++]!=")"){
		return false;
	}
	shared_ptr<Instruction> instructionTrue,instructionFalse;
	if(!ReadInstruction(vec,new_index,instructionTrue)){
		return false;
	}
	ifI=make_shared<If>();
	ifI->condition=expression;
	ifI->instructionTrue=instructionTrue;
	if(vec[new_index]=="else"){
		new_index++;
		if(!ReadInstruction(vec,new_index,instructionFalse)){
			return false;
		}
	}
	ifI->instructionFalse=instructionFalse;
	index=new_index;
	return true;
}
