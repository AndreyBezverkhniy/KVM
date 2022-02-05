#pragma once
#include "literal.h"
#include <vector>
#include <string>
#include <set>
#include "program.h"

bool Read1stLevelInstruction(const vector<Literal> &vec,int &index,
Program &program,set<string> modules);
bool ReadImport(const vector<Literal> &vec,int &index,Program &program,set<string> modules);
