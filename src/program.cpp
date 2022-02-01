#include "program.h"
#include <fstream>

Program::Program(){}
bool Program::Save(string path){
	ofstream fout;
	fout.open(path,ios_base::binary);
	if(!fout.is_open()){
		return false;
	}
	bool success;
	success=Save(fout);
	fout.close();
	return success;
}
bool Program::Load(string path){
	ifstream fin;
	fin.open(path,ios_base::binary);
	if(!fin.is_open()){
		return false;
	}
	bool success;
	success=Load(fin);
	fin.close();
	return success;
}
bool Program::Save(ostream &os){
	return global.Save(os);
}
bool Program::Load(istream &is){
	return global.Load(is);
}
