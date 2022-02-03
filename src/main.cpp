#include <iostream>
#include "program.h"
#include "var_declaration.h"
#include "expression.h"
#include "bin_operator.h"
#include "operand.h"
#include "simple_expression.h"
#include "left_unary_operator.h"
#include "right_unary_operator.h"

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	Body body;
	shared_ptr<LeftUnaryOperator> instruction1=make_shared<LeftUnaryOperator>();
	instruction1->SetOperand(make_shared<SimpleExpression>());
	instruction1->SetOperation(LEFT_UNARY_NOT);
	body.instructions.push_back(instruction1);
	shared_ptr<RightUnaryOperator> instruction2=make_shared<RightUnaryOperator>();
	instruction2->SetOperand(make_shared<SimpleExpression>());
	instruction2->SetOperation(RIGHT_UNARY_INC);
	body.instructions.push_back(instruction2);
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
