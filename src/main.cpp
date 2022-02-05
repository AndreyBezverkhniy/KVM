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
	string text="\"str\"";
	istringstream is(text);
	vector<Literal> vec;
	int res=ReadFile(is,vec);
	cout<<res<<" ";
	for(auto e:vec){
		cout<<e.ToString();
	}
	cout<<endl;
	cout<<"OK"<<endl;
	return 0;
}
