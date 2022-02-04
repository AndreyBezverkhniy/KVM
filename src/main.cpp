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

using namespace std;

int main(int argc, char* argv[]) {
	Program program;
	program=Program();
	if(!program.Load("program0.prog")){
		cout<<"error"<<endl;
	}

	program.Save(cout);
	cout<<endl;

	program.Save("program.prog");

	program=Program();
	program.Load("program.prog");

	program.Save(cout);
	cout<<endl;

	cout<<"OK"<<endl;
	return 0;
}
