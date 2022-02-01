#include <iostream>
#include <program.h>

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	program1.global.SetKeyValue("var1",96);
	program1.global.SetKeyValue("var2",-148);
	program1.functions[FunctionSignature("func1",1)]=Body();
	program1.functions[FunctionSignature("func2",2)]=Body();
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
