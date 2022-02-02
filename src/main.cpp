#include <iostream>
#include "program.h"
#include "var_declaration.h"
#include "expression.h"

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	Body body;
	shared_ptr<Expression> expression=make_shared<Expression>();
	shared_ptr<Instruction> instruction=dynamic_pointer_cast<Instruction>(expression);
	body.instructions.push_back(instruction);
	program1.functions[FunctionSignature("func",2)]=body;
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
