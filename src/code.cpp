#include "code.h"
#include "code_1st_level_instruction.h"
#include <fstream>
#include <set>

bool ReadFile(string path,vector<Lexeme> &vec){
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
bool ReadFile(istream &is,vector<Lexeme> &vec){
	Lexeme lexeme;
	int ch;
	while((ch=is.get(),is.good())){
		if(!lexeme.AddChar(ch)){
			if(lexeme.IsEmpty() || !lexeme.IsReady()){
				return false;
			}
			if(lexeme.type!=SPACE && lexeme.type!=ONELINE_COMMENT &&
			lexeme.type!=MULTILINE_COMMENT){
				vec.push_back(lexeme);
			}
			lexeme.Clean();
			if(!lexeme.AddChar(ch)){
				return false;
			}
		}
	}
	if(!lexeme.IsEmpty()){
		if(!lexeme.IsReady()){
			return false;
		}
		if(lexeme.type!=SPACE && lexeme.type!=ONELINE_COMMENT &&
			lexeme.type!=MULTILINE_COMMENT){
			vec.push_back(lexeme);
		}
	}
	lexeme.Clean();
	vec.push_back(lexeme);
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
	vector<Lexeme> vec;
	if(!ReadFile(is,vec)){
		return false;
	}
	for(auto e:vec){
		cout<<e.ToString();
	}
	cout<<endl;
	int index=0;
	Lexeme empty;
	while(vec[index]!=empty){
		if(!Read1stLevelInstruction(vec,index,program,modules)){
			return false;
		}
	}
	return true;
}
