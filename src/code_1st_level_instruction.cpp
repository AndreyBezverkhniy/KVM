#include "code.h"
#include "utils.h"
#include "inbuilt_functions.h"

bool Code::Read1stLevelInstruction(const vector<Lexeme> &vec,int &index){
	if(ReadImport(vec,index)){
		return true;
	}
	if(ReadGlobal(vec,index)){
		return true;
	}
	if(ReadFunctionDeclaration(vec,index)){
		return true;
	}
	return false;
}
bool Code::ReadImport(const vector<Lexeme> &vec,int &index){
	if(vec[index]=="import" && vec[index+1].type==STRING && vec[index+2]==";"){
		string module_path=vec[index+1].str;
		index+=3;
		if(modules.find(module_path)==modules.end()){
			return ReadProgramModule(module_path);
		} else {
			return true;
		}
	}
	return false;
}
bool Code::ReadGlobal(const vector<Lexeme> &vec,int &index){
	int new_index=index;
	if(vec[new_index++]!="global"){
		return false;
	}
	string identifier;
	while(IsIdentifier(identifier=vec[new_index].str)){
		new_index++;
		int init_value=0;
		if(vec[new_index]=="="){
			if(IsNumber(vec[new_index+1].str)){
				init_value=ToInt(vec[new_index+1].str);
				new_index+=2;
			} else if(vec[new_index+1].str=="-" && IsNumber(vec[new_index+2].str)){
				init_value=-ToInt(vec[new_index+2].str);
				new_index+=3;
			}
		}
		if(program.global.Have(identifier)){
			return false;
		}
		program.global.SetValueInContext(identifier,init_value);
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
bool Code::ReadFunctionDeclaration(const vector<Lexeme> &vec,int &index){
	int new_index=index;
	if(vec[new_index++]!="function"){
		return false;
	}
	FunctionSignature signature;
	Body body;
	signature.func_name=vec[new_index++].str;
	if(!IsIdentifier(signature.func_name)){
		return false;
	}
	if(vec[new_index++].str!="("){
		return false;
	}
	string identifier;
	if(vec[new_index].str!=")"){
		for(;;){
			identifier=vec[new_index].str;
			if(!IsIdentifier(identifier)){
				return false;
			}
			body.argument_names.push_back(identifier);
			new_index++;
			if(vec[new_index].str!=","){
				break;
			}
			new_index++;
		}
	}
	if(vec[new_index++].str!=")"){
		return false;
	}
	signature.arg_n=body.argument_names.size();
	if(signature==function_read || signature==function_write){
		cout<<"redeclaration of inbuilt function "<<signature.func_name
		<<"("<<signature.arg_n<<")"<<endl;
		return false;
	}
	for(auto pair:program.functions){
		auto func_signature=pair.first;
		if(signature==func_signature){
			throw CompileTimeErrorException("function redeclaration",vec[index+1]);
		}
		if(signature.func_name=="main" && func_signature.func_name=="main"){
			throw CompileTimeErrorException("another main function declaration",vec[index+1]);
		}
	}
	shared_ptr<Block> block;
	if(!ReadBlock(vec,new_index,block)){
		return false;
	}
	body.block=block;
	program.functions[signature]=body;
	index=new_index;
	return true;
}
