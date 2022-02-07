#pragma once
#include <vector>
#include <iostream>
#include <string>
#include "lexeme.h"
#include "program.h"
#include <set>

using namespace std;

bool ReadFile(string path,vector<Lexeme> &vec);
bool ReadFile(istream &is,vector<Lexeme> &vec);
bool ReadProgram(string module_path,Program &program);
bool ReadProgram(istream &is,Program &program);
bool ReadProgramModule(string module_path,Program &program,set<string> &modules);
bool ReadProgramModule(istream &is,Program &program,set<string> modules);
