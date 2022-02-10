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
	shared_ptr<Context> global_context;
	Executor();
	int exec();
	void exec_block(Block *block);
	void exec_instruction(Instruction *instruction);
	int exec_expression(Expression *expression);
	int exec_bin(BinOperator *bin);
	int exec_operand(Operand *operand);
	int exec_lunary(LeftUnaryOperator *lunar);
	int exec_runary(RightUnaryOperator *runar);
	int exec_simple(SimpleExpression *simple_expression);
	int exec_number(Number *number);
	int exec_variable(VariableName *varibable);
	int exec_fcall(FunctionCall *fcall);
	// void exec_var(shared_ptr<Var> &var);
	// void exec_return(shared_ptr<Return> &ret);
	// void exec_if(shared_ptr<If> &ifI);
	// void exec_while(shared_ptr<While> &whileI);
};
