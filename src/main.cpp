#include <iostream>
#include "program.h"
#include "var_declaration.h"

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	program1.global.SetKeyValue("var1",96);
	program1.global.SetKeyValue("var2",-148);
	program1.functions[FunctionSignature("func1",1)]=Body();
	Body body;
	shared_ptr<Var> var=make_shared<Var>();
	var->declarations.push_back(pair<string,int>("v1",13));
	var->declarations.push_back(pair<string,int>("v2",24));
	shared_ptr<Instruction> instruction=dynamic_pointer_cast<Instruction>(var);
	body.instructions.push_back(instruction);
	program1.functions[FunctionSignature("func2",2)]=body;
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
