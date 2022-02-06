#include "program.h"
#include <fstream>
#include "utils.h"

Program::Program(){}
bool Program::Save(string path) const {
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
bool Program::Save(ostream &os) const {
	if(!global.Save(os)){
		return false;
	}
	if(!USave(os,(int)functions.size())){
		return false;
	}
	for(auto pair:functions){
		if(!pair.first.Save(os)){
			return false;
		}
		if(!pair.second.SaveInner(os)){
			return false;
		}
	}
	return true;
}
bool Program::Load(istream &is){
	if(!global.Load(is)){
		return false;
	}
	int size;
	if(!ULoad(is,size)){
		return false;
	}
	FunctionSignature key;
	shared_ptr<Block> value;
	for(int i=0;i<size;i++){
		if(!key.Load(is)){
			return false;
		}
		value=make_shared<Block>();
		if(!value->LoadInner(is)){
			return false;
		}
		functions[key]=*value;
	}
	return true;
}
