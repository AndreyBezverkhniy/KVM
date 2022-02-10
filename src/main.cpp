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
#include "code.h"
#include <sstream>
#include "executor.h"

using namespace std;

int main(int argc, char* argv[]) {
	string text=" \
global a=1, c=10; \
function main(a,b){ \
	1>2; \
	1>=2; \
	1<=2; \
	1<2; \
	1==2; \
	1!=2; \
	11+2; \
	11-2; \
	11*2; \
	11/2; \
	11%2; \
	2||0; \
	2&&3; \
} \
";

	cout<<text<<endl;
	istringstream is(text);
	Executor executor;
	if(!ReadProgram(is,executor.program)){
		cout<<endl<<"ReadProgram Failed"<<endl;
	}
	executor.program.Save(cout);
	cout<<"/1"<<endl;
	executor.program.Save("program.prog");
	Program program2;
	program2.Load("program.prog");
	program2.Save(cout);
	cout<<"/2"<<endl;
	cout<<"RUN"<<endl;
	executor.exec();
	cout<<"END"<<endl;
	return 0;
}
