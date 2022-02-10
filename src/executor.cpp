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
	shared_ptr<FunctionCall> main_call=make_shared<FunctionCall>();
	main_call->signature=main_signature;
	for(int i=0;i<main_signature.arg_n;i++){
		main_call->arguments.push_back(make_shared<Number>(0));
	}
	global_context=make_shared<Context>(program.global);
	current_context=global_context;
	exec_fcall(main_call.get());
	return 0;
}
void Executor::exec_instruction(Instruction *instruction){
	if(auto ptr=dynamic_cast<Expression*>(instruction)){
		/*return/**/ cout<<exec_expression(ptr)<<endl;
		return;
	}
	cout<<"instruction: no handler"<<endl;
}
void Executor::exec_block(Block *block){
	shared_ptr<Context> block_context=make_shared<Context>();
	block_context->SetParentContext(current_context);
	current_context=block_context;
	// PrintContextChain(current_context);
	// handle block
	for(shared_ptr<Instruction> instruction:block->instructions){
		exec_instruction(instruction.get());
	}
	current_context=current_context->GetParentContext();
}
int Executor::exec_expression(Expression *expression){
	if(auto ptr=dynamic_cast<SimpleExpression*>(expression)){
		return exec_simple(ptr);
	}
	cout<<"incorrect expression"<<endl;
}
int Executor::exec_simple(SimpleExpression *simple){
	if(auto ptr=dynamic_cast<Number*>(simple)){
		return exec_number(ptr);
	}
	if(auto ptr=dynamic_cast<VariableName*>(simple)){
		return exec_variable(ptr);
	}
	if(auto ptr=dynamic_cast<FunctionCall*>(simple)){
		return exec_fcall(ptr);
	}
	cout<<"incorrect simple"<<endl;
}
int Executor::exec_number(Number *number){
	return number->n;
}
int Executor::exec_variable(VariableName *variable){
	int value;
	shared_ptr<Context> context=current_context;
	bool name_exist=false;
	string &name=variable->name;
	while(context){
		if(context->Have(name)){
			name_exist=true;
			value=context->GetValue(name);
			break;
		}
		context=context->GetParentContext();
	}
	if(!name_exist){
		cout<<"try to access variable by unknown name "<<variable->name<<endl;
	}
	return value;
}
int Executor::exec_fcall(FunctionCall *fcall){
	int return_value=0;
	shared_ptr<Context> arguments_context=make_shared<Context>();
	const vector<string> &arg_names=program.functions[fcall->signature].argument_names;
	for(int i=0;i<arg_names.size();i++){
		arguments_context->SetKeyValue(arg_names[i],exec_expression(fcall->arguments[i].get()));
	}
	arguments_context->SetParentContext(global_context);
	shared_ptr<Context> old_current_context=current_context;
	current_context=arguments_context;
	exec_block(program.functions[fcall->signature].block.get());
	current_context=old_current_context;
	return return_value;
}
void PrintContextChain(shared_ptr<Context> ptr){
	if(!ptr){
		cout<<"Null Context Pointer"<<endl;
		return;
	}
	PrintContextChain(ptr->parent);
	cout<<"{ ";
	for(auto e:ptr->variables){
		cout<<e.first<<"="<<e.second<<" ";
	}
	cout<<"}"<<endl;
}
