#include "executor.h"
#include "return_exception.h"
#include "inbuilt_functions.h"
#include "run_time_error_exception.h"

Executor::Executor(){}
int Executor::exec(){
	try{
		bool main_exists=false;
		FunctionSignature main_signature;
		for(auto key_value:program.functions){
			if(key_value.first.func_name=="main"){
				main_signature=key_value.first;
				main_exists=true;
			}
		}
		if(!main_exists){
			throw RunTimeErrorException("main function has no place to be");
		}
		shared_ptr<FunctionCall> main_call=make_shared<FunctionCall>();
		main_call->signature=main_signature;
		for(int i=0;i<main_signature.arg_n;i++){
			main_call->arguments.push_back(make_shared<Number>(0));
		}
		global_context=make_shared<Context>(program.global);
		current_context=global_context;
		exec_fcall(main_call.get());
	} catch (RunTimeErrorException e){
		cout<<"KRTE: "<<e.what()<<endl;
		if(instruction_stack_trace.size()>0){
			cout<<"instruction stack trace:"<<endl;
			for(auto place:instruction_stack_trace){
				cout<<"\t"<<place.first<<":"<<place.second<<endl;
			}
		}
	}
	return 0;
}
void Executor::exec_instruction(Instruction *instruction){
	if(auto ptr=dynamic_cast<Expression*>(instruction)){
		exec_expression(ptr);
	} else if(auto ptr=dynamic_cast<Block*>(instruction)){
		exec_block(ptr,"{block}");
	} else if(auto ptr=dynamic_cast<If*>(instruction)){
		exec_if(ptr);
	} else if(auto ptr=dynamic_cast<While*>(instruction)){
		exec_while(ptr);
	} else if(auto ptr=dynamic_cast<Return*>(instruction)){
		exec_return(ptr);
	} else if(auto ptr=dynamic_cast<Var*>(instruction)){
		exec_var(ptr);
	} else {
		throw RunTimeErrorException("unknown instruction");
	}
}
void Executor::exec_block(Block *block,string block_name){
	instruction_stack_trace.push_back(pair<string,int>(block_name,0));
	shared_ptr<Context> block_context=make_shared<Context>();
	block_context->SetParentContext(current_context);
	current_context=block_context;
	int trace_index=instruction_stack_trace.size()-1;
	for(shared_ptr<Instruction> instruction:block->instructions){
		instruction_stack_trace[trace_index].second++;
		exec_instruction(instruction.get());
	}
	current_context=current_context->GetParentContext();
	instruction_stack_trace.pop_back();
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
void Executor::exec_return(Return *ret){
	int value=exec_expression(ret->expression.get());
	throw ReturnException(value);
}
void Executor::exec_var(Var *var){
	for(auto e:var->declarations){
		string &name=e.first;
		int value=exec_expression(e.second.get());
		current_context->SetValueInContext(name,value);
	}
}
int Executor::exec_expression(Expression *expression){
	if(auto ptr=dynamic_cast<Operand*>(expression)){
		return exec_operand(ptr);
	}
	if(auto ptr=dynamic_cast<BinOperator*>(expression)){
		return exec_bin(ptr);
	}
	throw RunTimeErrorException("incorrect expression");
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
	} else if(operation[operation.size()-1]=='='){
		auto variable=dynamic_cast<VariableName*>(bin->left.get());
		if(!variable){
			cout<<"bin operator =/X=: operand is not a variable"<<endl;
			return 0;
		}
		string name=variable->name;
		if(!current_context->Have(name)){
			throw RunTimeErrorException("bin operator =/X=: variable is not exist");
		}
		if(operation=="="){
			return_value=right;
		} else if(operation=="+="){
			return_value=left+right;
		} else if(operation=="-="){
			return_value=left-right;
		} else if(operation=="*="){
			return_value=left*right;
		} else if(operation=="/="){
			return_value=left/right;
		} else if(operation=="%="){
			return_value=left%right;
		} else {
			throw RunTimeErrorException("unknown bin");
		}
		current_context->SetValueInChain(name,return_value);
	} else {
		throw RunTimeErrorException("unknown bin");
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
	throw RunTimeErrorException("incorrect operand");
}
int Executor::exec_lunary(LeftUnaryOperator *lunar){
	int return_value;
	const string &operation=lunar->operation;
	if(operation=="++" || operation=="--"){
		auto variable=dynamic_cast<VariableName*>(lunar->operand.get());
		if(!variable){
			throw RunTimeErrorException("lunary operator inc/dec: operand is not a variable");
		}
		string name=variable->name;
		if(!current_context->Have(name)){
			throw RunTimeErrorException("lunary operator inc/dec: variable is not exist");
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
			throw RunTimeErrorException("unknown lunary");
		}
	}
	return return_value;
}
int Executor::exec_runary(RightUnaryOperator *runar){
	int return_value;
	const string &operation=runar->operation;
	auto variable=dynamic_cast<VariableName*>(runar->operand.get());
	if(!variable){
		throw RunTimeErrorException("runary operator inc/dec: operand is not a variable");
	}
	string name=variable->name;
	if(!current_context->Have(name)){
		throw RunTimeErrorException("runary operator inc/dec: variable is not exist");
	}
	int old_value=current_context->GetValueInChain(name);
	if(operation=="++"){
		current_context->SetValueInChain(name,old_value+1);
		return_value=old_value;
	} else if (operation=="--"){
		current_context->SetValueInChain(name,old_value-1);
		return_value=old_value;
	} else {
		throw RunTimeErrorException("unknown runary");
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
	throw RunTimeErrorException("incorrect simple");
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
		throw RunTimeErrorException("try to access variable by unknown name ");
	}
	return value;
}
int Executor::exec_fcall(FunctionCall *fcall){
	int return_value=0;
	vector<int> argument_values;
	for(int i=0;i<fcall->signature.arg_n;i++){
		argument_values.push_back(exec_expression(fcall->arguments[i].get()));
	}
	shared_ptr<Context> arguments_context=make_shared<Context>();
	auto iter=program.functions.find(fcall->signature);
	if(iter==program.functions.end()){
		if(fcall->signature==function_write){
			return exec_write(argument_values);
		}
		if(fcall->signature==function_read){
			return exec_read(argument_values);
		}
		string message;
		message="unknown function "+fcall->signature.func_name+"-"
		+to_string(fcall->signature.arg_n);
		throw RunTimeErrorException(message);
	}
	const vector<string> &arg_names=(*iter).second.argument_names;
	for(int i=0;i<fcall->signature.arg_n;i++){
		arguments_context->SetValueInContext(arg_names[i],argument_values[i]);
	}
	arguments_context->SetParentContext(global_context);
	shared_ptr<Context> old_current_context=current_context;
	current_context=arguments_context;
	try{
		string block_name;
		block_name=fcall->signature.func_name+"-"+to_string(fcall->signature.arg_n);
		exec_block(program.functions[fcall->signature].block.get(),block_name);
	} catch(ReturnException e){
		return_value=e.GetValue();
	}
	current_context=old_current_context;
	return return_value;
}
