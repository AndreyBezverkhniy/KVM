#pragma once
#include "lexeme.h"
#include <vector>
#include <string>
#include <set>
#include "program.h"

bool Read1stLevelInstruction(const vector<Lexeme> &vec,int &index,
Program &program,set<string> &modules);
bool ReadImport(const vector<Lexeme> &vec,int &index,Program &program,set<string> &modules);
bool ReadGlobal(const vector<Lexeme> &vec,int &index,Program &program);
bool ReadFunctionDeclaration(const vector<Lexeme> &vec,int &index,Program &program);
