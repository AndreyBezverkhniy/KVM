#include "code_1st_level_instruction.h"
#include "code.h"

bool Read1stLevelInstruction(const vector<Literal> &vec,int &index,
Program &program,set<string> modules){
	if(ReadImport(vec,index,program,modules)){
		return true;
	}
	return false;
}
bool ReadImport(const vector<Literal> &vec,int &index,Program &program,set<string> modules){
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
