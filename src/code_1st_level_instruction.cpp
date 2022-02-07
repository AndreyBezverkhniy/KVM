#include "code_1st_level_instruction.h"
#include "code.h"
#include "code_instruction.h"
#include "utils.h"
#include "function_signature.h"
#include "block.h"
#include <memory>

bool Read1stLevelInstruction(const vector<Lexeme> &vec,int &index,
Program &program,set<string> &modules){
	if(ReadImport(vec,index,program,modules)){
		return true;
	}
	if(ReadGlobal(vec,index,program)){
		return true;
	}
	if(ReadFunctionDeclaration(vec,index,program)){
		return true;
	}
	return false;
}
bool ReadImport(const vector<Lexeme> &vec,int &index,Program &program,set<string> &modules){
	if(vec[index]=="import" && vec[index+1].type==STRING && vec[index+2]==";"){
		string module_path=vec[index+1].str;
		index+=3;
		if(modules.find(module_path)==modules.end()){
			return ReadProgramModule(module_path,program,modules);
		} else {
			return true;
		}
	}
	return false;
}
bool ReadGlobal(const vector<Lexeme> &vec,int &index,Program &program){
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
		program.global.SetKeyValue(identifier,init_value);
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
bool ReadFunctionDeclaration(const vector<Lexeme> &vec,int &index,Program &program){
	int new_index=index;
	if(vec[new_index++]!="function"){
		return false;
	}
	FunctionSignature signature;
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
			signature.arguments.push_back(identifier);
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
	for(auto pair:program.functions){
		auto func_signature=pair.first;
		if(func_signature.func_name==signature.func_name && (signature.func_name=="main" ||
		func_signature.arguments.size()==signature.arguments.size())){
			return false;
		}
	}
	shared_ptr<Block> block;
	if(!ReadBlock(vec,new_index,block)){
		return false;
	}
	program.functions[signature]=*block;
	index=new_index;
	return true;
}
