#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <memory>
#include "lexeme.h"
#include "program.h"
#include "instruction.h"
#include "block.h"
#include "var_declaration.h"
#include "return.h"
#include "while.h"
#include "if.h"
#include "expression.h"
#include "bin_operator.h"
#include "operand.h"
#include "number.h"
#include "variable_name.h"
#include "function_call.h"
#include "left_unary_operator.h"
#include "right_unary_operator.h"
#include "compile_time_error_exception.h"

using namespace std;

class Code{
public:
	Program &program;
	set<string> modules;

	// program
	Code(Program &program);
	bool ReadFile(string path,vector<Lexeme> &vec);
	bool ReadFile(istream &is,vector<Lexeme> &vec);
	bool ReadProgram(string module_path);
	bool ReadProgram(istream &is);
	bool ReadProgramModule(string module_path);
	bool ReadProgramModule(istream &is);

	// 1st level instruction
	bool Read1stLevelInstruction(const vector<Lexeme> &vec,int &index);
	bool ReadImport(const vector<Lexeme> &vec,int &index);
	bool ReadGlobal(const vector<Lexeme> &vec,int &index);
	bool ReadFunctionDeclaration(const vector<Lexeme> &vec,int &index);

	// instruction
	bool ReadInstruction(const vector<Lexeme> &vec,int &index,
	shared_ptr<Instruction> &instruction);
	bool ReadSemicoloningInstruction(const vector<Lexeme> &vec,int &index,
	shared_ptr<Instruction> &instruction);
	bool ReadBlock(const vector<Lexeme> &vec,int &index,shared_ptr<Block> &block);
	bool ReadVar(const vector<Lexeme> &vec,int &index,shared_ptr<Var> &var);
	bool ReadReturn(const vector<Lexeme> &vec,int &index,shared_ptr<Return> &ret);
	bool ReadWhile(const vector<Lexeme> &vec,int &index,shared_ptr<While> &whileI);
	bool ReadIf(const vector<Lexeme> &vec,int &index,shared_ptr<If> &ifI);

	//expression
	bool ReadExpression(const vector<Lexeme> &vec,int &index,shared_ptr<Expression> &expression);
	bool ReadBinOperator(const vector<Lexeme> &vec,int &index,string &bin);
	bool ReadOperand(const vector<Lexeme> &vec,int &index,shared_ptr<Expression> &operand);
	bool ReadLeftUnaryOperator(const vector<Lexeme> &vec,int &index,string &lunar);
	bool ReadRightUnaryOperator(const vector<Lexeme> &vec,int &index,string &runar);
	bool ReadSimpleExpression(const vector<Lexeme> &vec,int &index,
	shared_ptr<Expression> &simple_expression);
	bool ReadNumber(const vector<Lexeme> &vec,int &index,shared_ptr<Number> &number);
	bool ReadVariableName(const vector<Lexeme> &vec,int &index,shared_ptr<VariableName> &variable);
	bool ReadFunctionCall(const vector<Lexeme> &vec,int &index,shared_ptr<FunctionCall> &fcall);
	bool ReadParenthesizedExpression(const vector<Lexeme> &vec,int &index,
	shared_ptr<Expression> &parenthesized);
	int GetBinOperandPriority(string bin);
	bool IsRightAssociativeBinOperator(string bin);
	bool BinOrder(string binl,string binr);
	shared_ptr<Expression> BuildExpression(vector<shared_ptr<Expression>> &ops,vector<string> &bins);
	bool IsBinOperator(string str);
	bool IsLeftUnaryOperator(string str);
	bool IsRightUnaryOperator(string str);
	bool IsOperator(string str,const vector<string> &values);
};
