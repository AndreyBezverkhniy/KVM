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

using namespace std;

int main(int argc, char* argv[]) {
	string text=	"function func(){ \
						1+22*333-4444; \
					}";
	istringstream is(text);
	Program program;
	if(!ReadProgram(is,program)){
		cout<<endl<<"ReadProgram Failed"<<endl;
	}
	program.Save(cout);
	cout<<"/1"<<endl;
	program.Save("program.prog");
	Program program2;
	program2.Load("program.prog");
	program2.Save(cout);
	cout<<"/2"<<endl;
	cout<<"OK"<<endl;
	return 0;
}
