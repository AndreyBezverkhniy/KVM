#include "code_1st_level_instruction.h"
#include "code.h"
#include "utils.h"

bool Read1stLevelInstruction(const vector<Literal> &vec,int &index,
Program &program,set<string> &modules){
	if(ReadImport(vec,index,program,modules)){
		return true;
	}
	if(ReadGlobal(vec,index,program)){
		return true;
	}
	return false;
}
bool ReadImport(const vector<Literal> &vec,int &index,Program &program,set<string> &modules){
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
bool ReadGlobal(const vector<Literal> &vec,int &index,Program &program){
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
