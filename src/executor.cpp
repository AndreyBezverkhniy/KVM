#include "executor.h"

Executor::Executor(){}
int Executor::exec(){
	bool main_exists=false;
	FunctionSignature main_signature;
	for(auto key_value:program.functions){
		if(key_value.first.func_name=="main"){
			main_signature=key_value.first;
			main_exists=true;
		}
	}
	if(!main_exists){
		cout<<"main function has no place to be"<<endl;
		return -1;
	}
	FunctionCall main_call;
	main_call.signature=main_signature;
	for(int i=0;i<main_signature.arg_n;i++){
		main_call.arguments.push_back(make_shared<Number>(0));
	}
	main_call.Save(cout);
	cout<<endl;
	return 0;
}
