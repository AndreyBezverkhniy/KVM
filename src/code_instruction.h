#pragma once
#include "literal.h"
#include <vector>
#include "instruction.h"
#include "block.h"
#include "var_declaration.h"
#include <memory>

using namespace std;

bool ReadInstruction(const vector<Literal> &vec,int &index,shared_ptr<Instruction> &instruction);
bool ReadBlock(const vector<Literal> &vec,int &index,shared_ptr<Block> &block);
bool ReadVar(const vector<Literal> &vec,int &index,shared_ptr<Var> &var);
bool ReadExpressionInstruction(const vector<Literal> &vec,int &index,shared_ptr<Expression> &expression);
