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

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	Body body;
	shared_ptr<VariableName> expression=make_shared<VariableName>();
	expression->SetName("vname1");
	shared_ptr<VariableName> condition=make_shared<VariableName>();
	condition->SetName("vname2");
	shared_ptr<Return> instruction=make_shared<Return>();
	instruction->SetExpression(expression);
	shared_ptr<While> whileI=make_shared<While>();
	whileI->SetCondition(condition);
	whileI->SetInstruction(instruction);
	body.instructions.push_back(whileI);
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
