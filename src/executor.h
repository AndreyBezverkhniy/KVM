#include "program.h"
#include "context.h"
#include "instruction.h"
#include "block.h"
#include "expression.h"
#include "bin_operator.h"
#include "operand.h"
#include "left_unary_operator.h"
#include "right_unary_operator.h"
#include "simple_expression.h"
#include "number.h"
#include "variable_name.h"
#include "function_call.h"
#include "var_declaration.h"
#include "return.h"
#include "if.h"
#include "while.h"

using namespace std;

class Executor{
public:
	Program program;
	shared_ptr<Context> current_context;
	Executor();
	int exec();
	// int exec_instruction(shared_ptr<Instruction> &instruction);
	// void exec_block(shared_ptr<Block> &block);
	// int exec_expression(shared_ptr<Expression> &expression`);
	// int exec_operand(shared_ptr<Operand> &operand);
	// int exec_simple(shared_ptr<SimpleExpression> &simple_expression);
	// int exec_number(shared_ptr<Number> &number);
	// int exec_variable(shared_ptr<VariableName> &varibable);
	// void exec_fcall(shared_ptr<FunctionCall> &fcall);
	// int exec_lunary(shared_ptr<LeftUnaryOperator> &lunar);
	// int exec_runary(shared_ptr<RightUnaryOperator> &runar);
	// int exec_bin(shared_ptr<BinOperator> &bin);
	// void exec_var(shared_ptr<Var> &var);
	// void exec_return(shared_ptr<Return> &ret);
	// void exec_if(shared_ptr<If> &ifI);
	// void exec_while(shared_ptr<While> &whileI);
};
