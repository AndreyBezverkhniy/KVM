#pragma once
#include "lexeme.h"
#include <vector>
#include "instruction.h"
#include "block.h"
#include "var_declaration.h"
#include "return.h"
#include "while.h"
#include "if.h"
#include <memory>

using namespace std;

bool ReadInstruction(const vector<Lexeme> &vec,int &index,shared_ptr<Instruction> &instruction);
bool ReadSemicoloningInstruction(const vector<Lexeme> &vec,int &index,shared_ptr<Instruction> &instruction);
bool ReadBlock(const vector<Lexeme> &vec,int &index,shared_ptr<Block> &block);
bool ReadVar(const vector<Lexeme> &vec,int &index,shared_ptr<Var> &var);
bool ReadReturn(const vector<Lexeme> &vec,int &index,shared_ptr<Return> &ret);
bool ReadWhile(const vector<Lexeme> &vec,int &index,shared_ptr<While> &whileI);
bool ReadIf(const vector<Lexeme> &vec,int &index,shared_ptr<If> &ifI);
