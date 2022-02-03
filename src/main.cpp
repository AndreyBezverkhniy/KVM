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

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	Body body;

	shared_ptr<VariableName> expression=make_shared<VariableName>();
	expression->SetName("vname1");
	shared_ptr<Return> instruction1=make_shared<Return>();
	instruction1->SetExpression(expression);
	shared_ptr<Number> instruction2=make_shared<Number>();
	instruction2->SetValue(123);

	shared_ptr<VariableName> condition=make_shared<VariableName>();
	condition->SetName("vname2");
	shared_ptr<If> if1=make_shared<If>();
	if1->SetCondition(condition);
	if1->SetTrueInstruction(instruction1);

	shared_ptr<If> if2else=make_shared<If>();
	if2else->SetCondition(condition);
	if2else->SetTrueInstruction(instruction1);
	if2else->SetFalseInstruction(instruction2);

	body.instructions.push_back(if1);
	body.instructions.push_back(if2else);
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
