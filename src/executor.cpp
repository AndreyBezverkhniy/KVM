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
		cout<<exec_expression(ptr)<<endl;
	} else if(auto ptr=dynamic_cast<Block*>(instruction)){
		exec_block(ptr);
	} else if(auto ptr=dynamic_cast<If*>(instruction)){
		exec_if(ptr);
	} else if(auto ptr=dynamic_cast<While*>(instruction)){
		exec_while(ptr);
	} else {
		cout<<"instruction: no handler"<<endl;
	}
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
void Executor::exec_if(If *ifI){
	int condition=exec_expression(ifI->condition.get());
	if(condition){
		exec_instruction(ifI->instructionTrue.get());
	} else if(ifI->instructionFalse){
		exec_instruction(ifI->instructionFalse.get());
	}
}
void Executor::exec_while(While *whileI){
	int condition;
	while(condition=exec_expression(whileI->condition.get())){
		exec_instruction(whileI->instruction.get());
	}
}
int Executor::exec_expression(Expression *expression){
	if(auto ptr=dynamic_cast<Operand*>(expression)){
		return exec_operand(ptr);
	}
	if(auto ptr=dynamic_cast<BinOperator*>(expression)){
		return exec_bin(ptr);
	}
	cout<<"incorrect expression"<<endl;
}
int Executor::exec_bin(BinOperator *bin){
	int return_value=0;
	const string &operation=bin->operation;
	int left=exec_expression(bin->left.get());
	int right=exec_expression(bin->right.get());
	if(operation=="+"){
		return_value=left+right;
	} else if(operation=="-"){
		return_value=left-right;
	} else if(operation=="*"){
		return_value=left*right;
	} else if(operation=="/"){
		return_value=left/right;
	} else if(operation=="%"){
		return_value=left%right;
	} else if(operation=="%"){
		return_value=left+right;
	} else if(operation=="<"){
		return_value=left<right;
	} else if(operation==">"){
		return_value=left>right;
	} else if(operation=="=="){
		return_value=left==right;
	} else if(operation=="!="){
		return_value=left!=right;
	} else if(operation=="<="){
		return_value=left<=right;
	} else if(operation==">="){
		return_value=left>=right;
	} else if(operation=="&&"){
		return_value=left&&right;
	} else if(operation=="||"){
		return_value=left||right;
	} else if(operation=="="){
		return_value=left+right;///
	} else if(operation=="+="){
		return_value=left+right;///
	} else if(operation=="-="){
		return_value=left+right;///
	} else if(operation=="*="){
		return_value=left+right;///
	} else if(operation=="/="){
		return_value=left+right;///
	} else if(operation=="%="){
		return_value=left+right;///
	} else {
		cout<<"unknown lunary"<<endl;
	}
	return return_value;
}
int Executor::exec_operand(Operand *operand){
	if(auto ptr=dynamic_cast<SimpleExpression*>(operand)){
		return exec_simple(ptr);
	}
	if(auto ptr=dynamic_cast<LeftUnaryOperator*>(operand)){
		return exec_lunary(ptr);
	}
	if(auto ptr=dynamic_cast<RightUnaryOperator*>(operand)){
		return exec_runary(ptr);
	}
	cout<<"incorrect operand"<<endl;
}
int Executor::exec_lunary(LeftUnaryOperator *lunar){
	int return_value;
	const string &operation=lunar->operation;
	if(operation=="++" || operation=="--"){
		auto variable=dynamic_cast<VariableName*>(lunar->operand.get());
		if(!variable){
			cout<<"lunary operator inc/dec: operand is not a variable"<<endl;
			return 0;
		}
		string name=variable->name;
		if(!current_context->Have(name)){
			cout<<"lunary operator inc/dec: variable is not exist"<<endl;
			return 0;
		}
		int old_value=current_context->GetValueInChain(name);
		if(operation=="++"){
			current_context->SetValueInChain(name,old_value+1);
			return_value=old_value+1;
		} else {
			current_context->SetValueInChain(name,old_value-1);
			return_value=old_value-1;
		}
	} else {
		return_value=exec_expression(lunar->operand.get());
		if(operation=="+"){
			return_value=return_value;
		} else if(operation=="-"){
			return_value=-return_value;
		} else if(operation=="!"){
			return_value=(return_value?0:1);
		} else {
			cout<<"unknown lunary"<<endl;
		}
	}
	return return_value;
}
int Executor::exec_runary(RightUnaryOperator *runar){
	int return_value;
	const string &operation=runar->operation;
	auto variable=dynamic_cast<VariableName*>(runar->operand.get());
	if(!variable){
		cout<<"runary operator inc/dec: operand is not a variable"<<endl;
		return 0;
	}
	string name=variable->name;
	if(!current_context->Have(name)){
		cout<<"runary operator inc/dec: variable is not exist"<<endl;
		return 0;
	}
	int old_value=current_context->GetValueInChain(name);
	if(operation=="++"){
		current_context->SetValueInChain(name,old_value+1);
		return_value=old_value;
	} else if (operation=="--"){
		current_context->SetValueInChain(name,old_value-1);
		return_value=old_value;
	} else {
		cout<<"unknown lunary"<<endl;
	}
	return return_value;
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
			value=context->GetValueInChain(name);
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
		arguments_context->SetValueInContext(arg_names[i],exec_expression(fcall->arguments[i].get()));
	}
	arguments_context->SetParentContext(global_context);
	shared_ptr<Context> old_current_context=current_context;
	current_context=arguments_context;
	exec_block(program.functions[fcall->signature].block.get());
	current_context=old_current_context;
	return return_value;
}
