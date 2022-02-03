#include <iostream>
#include "program.h"
#include "var_declaration.h"
#include "expression.h"
#include "bin_operator.h"
#include "operand.h"
#include "element.h"

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	Body body;
	shared_ptr<Element> element=make_shared<Element>();
	body.instructions.push_back(element);
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
