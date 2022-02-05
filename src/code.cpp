#include "code.h"

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
	char ch;
	while((ch=is.get())!=EOF){
		if(!literal.AddChar(ch)){
			if(literal.IsEmpty() || !literal.IsReady()){
				return false;
			}
			vec.push_back(literal);
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
		vec.push_back(literal);
	}
	literal.Clean();
	vec.push_back(literal);
    return true;
}
