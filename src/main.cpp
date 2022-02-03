#include <iostream>
#include "program.h"
#include "var_declaration.h"
#include "expression.h"
#include "bin_operator.h"
#include "operand.h"
#include "element.h"
#include "left_unary_operator.h"

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	Body body;
	shared_ptr<LeftUnaryOperator> instruction=make_shared<LeftUnaryOperator>();
	instruction->SetOperand(make_shared<Element>());
	instruction->SetOperation(LEFT_UNARY_PLUS);
	body.instructions.push_back(instruction);
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
