#include "code.h"
#include <fstream>

Code::Code(Program &program):program(program){}
bool Code::ReadFile(string path,vector<Lexeme> &vec){
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
bool Code::ReadFile(istream &is,vector<Lexeme> &vec){
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
bool Code::ReadProgram(string module_path){
	modules=set<string>();
	bool result=false;
	try{
		result=ReadProgramModule(module_path);
	} catch (CompileTimeErrorException e){
		cout<<e.what()<<endl;
	}
	return result;
}
bool Code::ReadProgram(istream &is){
	modules=set<string>();
	bool result=false;
	try{
		result=ReadProgramModule(is);
	} catch (CompileTimeErrorException e){
		cout<<e.what()<<endl;
	}
	return result;
}
bool Code::ReadProgramModule(string module_path){
	ifstream fin;
	fin.open(module_path,ios_base::binary);
	if(!fin.is_open()){
		return false;
	}
	modules.insert(module_path);
	bool success=ReadProgramModule(fin);
	fin.close();
	return success;
}
bool Code::ReadProgramModule(istream &is){
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
		if(!Read1stLevelInstruction(vec,index)){
			return false;
		}
	}
	return true;
}
