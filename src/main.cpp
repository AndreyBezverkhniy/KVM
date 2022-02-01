#include <iostream>
#include <program.h>

using namespace std;

int main(int argc, char* argv[]) {
	Program program1;
	program1.global.SetKeyValue("var1",96);
	program1.global.SetKeyValue("var2",-148);
	program1.Save("program.prog");
	Program program2;
	program2.Load("program.prog");
	program2.Save(cout);
	//cout<<endl;
	cout<<endl<<"OK"<<endl;
	return 0;
}
