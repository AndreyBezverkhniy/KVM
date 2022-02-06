#include "code.h"
#include "code_1st_level_instruction.h"
#include <fstream>
#include <set>

bool ReadFile(string path,vector<Literal> &vec){
    ifstream fin;
	fin.open(path,ios_base::binary);
	if(!fin.is_open()){
		return false;
	}
	bool success;
	success=ReadFile(fin,vec);
	fin.close();
	return success;
}
bool ReadFile(istream &is,vector<Literal> &vec){
	Literal literal;
	int ch;
	while((ch=is.get(),is.good())){
		if(!literal.AddChar(ch)){
			if(literal.IsEmpty() || !literal.IsReady()){
				return false;
			}
			if(literal.type!=SPACE && literal.type!=ONELINE_COMMENT &&
			literal.type!=MULTILINE_COMMENT){
				vec.push_back(literal);
			}
			literal.Clean();
			if(!literal.AddChar(ch)){
				return false;
			}
		}
	}
	if(!literal.IsEmpty()){
		if(!literal.IsReady()){
			return false;
		}
		if(literal.type!=SPACE && literal.type!=ONELINE_COMMENT &&
			literal.type!=MULTILINE_COMMENT){
			vec.push_back(literal);
		}
	}
	literal.Clean();
	vec.push_back(literal);
    return true;
}
bool ReadProgram(string module_path,Program &program){
	set<string> modules;
	return ReadProgramModule(module_path,program,modules);
}
bool ReadProgram(istream &is,Program &program){
	return ReadProgramModule(is,program,set<string>());
}
bool ReadProgramModule(string module_path,Program &program,set<string> &modules){
	ifstream fin;
	fin.open(module_path,ios_base::binary);
	if(!fin.is_open()){
		return false;
	}
	modules.insert(module_path);
	bool success=ReadProgramModule(fin,program,modules);
	fin.close();
	return success;
}
bool ReadProgramModule(istream &is,Program &program,set<string> modules){
	vector<Literal> vec;
	if(!ReadFile(is,vec)){
		return false;
	}
	for(auto e:vec){
		cout<<e.ToString();
	}
	cout<<endl;
	int index=0;
	Literal empty;
	while(vec[index]!=empty){
		if(!Read1stLevelInstruction(vec,index,program,modules)){
			return false;
		}
	}
	return true;
}
	