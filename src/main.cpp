#include <iostream>
#include "program.h"
#include "var_declaration.h"
#include "expression.h"
#include "bin_operator.h"
#include "operand.h"
#include "simple_expression.h"
#include "left_unary_operator.h"
#include "right_unary_operator.h"
#include "number.h"
#include "variable_name.h"
#include "function_call.h"
#include "return.h"
#include "while.h"
#include "if.h"
#include "block.h"

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	Block body;
	shared_ptr<VariableName> expression1=make_shared<VariableName>();
	expression1->SetName("vname1");
	shared_ptr<Return> instruction1=make_shared<Return>();
	instruction1->SetExpression(expression1);
	shared_ptr<Number> instruction2=make_shared<Number>();
	instruction2->SetValue(123);
	shared_ptr<Block> block=make_shared<Block>();
	block->instructions.push_back(instruction1);
	block->instructions.push_back(instruction2);
	body.instructions.push_back(block);
	program1.functions[FunctionSignature("func",0)]=body;
	program1.Save(cout);
	cout<<endl;
	program1.Save("program.prog");
	Program program2;
	if(!program2.Load("program.prog")){
		cout<<"error"<<endl;
	}
	program2.Save(cout);
	cout<<endl;
	cout<<"OK"<<endl;
	return 0;
}
