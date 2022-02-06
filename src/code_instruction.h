#pragma once
#include "literal.h"
#include <vector>
#include "instruction.h"
#include "block.h"
#include <memory>

using namespace std;

bool ReadInstruction(const vector<Literal> &vec,int &index,shared_ptr<Instruction> &instruction);
bool ReadBlock(const vector<Literal> &vec,int &index,shared_ptr<Block> &block);
