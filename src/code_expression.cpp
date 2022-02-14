#include "code.h"
#include "utils.h"
#include <stack>
#include <algorithm>

bool Code::ReadExpression(const vector<Lexeme> &vec,int &index,shared_ptr<Expression> &expression){
    int new_index=index;
    vector<shared_ptr<Expression>> ops;
    vector<string> bins;
    shared_ptr<Expression> operand;
    string bin;
    if(!ReadOperand(vec,new_index,operand)){
        return false;
    }
    ops.push_back(operand);
	int end_index=new_index;
    while(ReadBinOperator(vec,end_index,bin)){
        if(!ReadOperand(vec,end_index,operand)){
            return false;
        }
        bins.push_back(bin);
        ops.push_back(operand);
		new_index=end_index;
    }
	expression=BuildExpression(ops,bins);
    index=new_index;
    return true;
}
bool Code::ReadBinOperator(const vector<Lexeme> &vec,int &index,string &bin){
	if(!IsBinOperator(vec[index].str)){
		return false;
	}
	bin=vec[index].str;
	index++;
	return true;
}
bool Code::ReadOperand(const vector<Lexeme> &vec,int &index,shared_ptr<Expression> &operand){
	int new_index=index;
	vector<string> lunars,runars;
	shared_ptr<Expression> simple_expression;
	string unary;
	while(ReadLeftUnaryOperator(vec,new_index,unary)){
		lunars.push_back(unary);
	}
	if(!ReadSimpleExpression(vec,new_index,simple_expression)){
		return false;
	}
	while(ReadRightUnaryOperator(vec,new_index,unary)){
		runars.push_back(unary);
	}
	shared_ptr<Expression> expression=simple_expression;
	shared_ptr<LeftUnaryOperator> left_temp_ptr;
	shared_ptr<RightUnaryOperator> right_temp_ptr;
	for(int i=lunars.size()-1;i>=0;i--){
		left_temp_ptr=make_shared<LeftUnaryOperator>();
		left_temp_ptr->SetOperation(lunars[i]);
		left_temp_ptr->SetOperand(expression);
		expression=left_temp_ptr;
	}
	for(int i=0;i<runars.size();i++){
		right_temp_ptr=make_shared<RightUnaryOperator>();
		right_temp_ptr->SetOperation(runars[i]);
		right_temp_ptr->SetOperand(expression);
		expression=right_temp_ptr;
	}
	operand=expression;
	index=new_index;
	return true;
}
bool Code::ReadLeftUnaryOperator(const vector<Lexeme> &vec,int &index,string &lunar){
	if(!IsLeftUnaryOperator(vec[index].str)){
		return false;
	}
	lunar=vec[index].str;
	index++;
	return true;
}
bool Code::ReadRightUnaryOperator(const vector<Lexeme> &vec,int &index,string &runar){
	if(!IsRightUnaryOperator(vec[index].str)){
		return false;
	}
	runar=vec[index].str;
	index++;
	return true;
}
bool Code::ReadSimpleExpression(const vector<Lexeme> &vec,int &index,
shared_ptr<Expression> &simple_expression){
	int new_index=index;
	shared_ptr<Expression> parehthesized;
	if(ReadParenthesizedExpression(vec,new_index,parehthesized)){
		simple_expression=parehthesized;
		index=new_index;
		return true;
	}
	shared_ptr<Number> number;
	if(ReadNumber(vec,new_index,number)){
		simple_expression=number;
		index=new_index;
		return true;
	}
	shared_ptr<FunctionCall> fcall;
	if(ReadFunctionCall(vec,new_index,fcall)){
		simple_expression=fcall;
		index=new_index;
		return true;
	}
	shared_ptr<VariableName> variable;
	if(ReadVariableName(vec,new_index,variable)){
		simple_expression=variable;
		index=new_index;
		return true;
	}
	return false;
}
bool Code::ReadParenthesizedExpression(const vector<Lexeme> &vec,int &index,
shared_ptr<Expression> &parenthesized){
	int new_index=index;
	if(vec[new_index++].str!="("){
		return false;
	}
	shared_ptr<Expression> expression;
	if(!ReadExpression(vec,new_index,expression)){
		return false;
	}
	if(vec[new_index++].str!=")"){
		return false;
	}
	index=new_index;
	parenthesized=expression;
	return true;
}
bool Code::ReadNumber(const vector<Lexeme> &vec,int &index,shared_ptr<Number> &number){
	if(!IsNumber(vec[index].str)){
		return false;
	}
	number=make_shared<Number>(ToInt(vec[index].str));
	index=index+1;
	return true;
}
bool Code::ReadFunctionCall(const vector<Lexeme> &vec,int &index,shared_ptr<FunctionCall> &fcall){
	int new_index=index;
	string fname;
	if(!IsIdentifier((fname=vec[new_index++].str))){
		return false;
	}
	shared_ptr<FunctionCall> ptr=make_shared<FunctionCall>();
	if(vec[new_index++]!="("){
		return false;
	}
	shared_ptr<Expression> expression;
	if(vec[new_index]!=")"){
		for(;;){
			if(!ReadExpression(vec,new_index,expression)){
				return false;
			}
			ptr->arguments.push_back(expression);
			if(vec[new_index]!=","){
				break;
			}
			new_index++;
		}
	}
	if(vec[new_index++]!=")"){
		return false;
	}
	ptr->signature.func_name=fname;
	ptr->signature.arg_n=ptr->arguments.size();
	fcall=ptr;
	index=new_index;
	return true;
}
bool Code::ReadVariableName(const vector<Lexeme> &vec,int &index,shared_ptr<VariableName> &variable){
	if(!IsIdentifier(vec[index].str)){
		return false;
	}
	variable=make_shared<VariableName>(vec[index].str);
	index=index+1;
	return true;
}
int Code::GetBinOperandPriority(string bin){
	if(IsRightAssociativeBinOperator(bin)){
		return 1;
	}
	if(bin=="||"){
		return 2;
	}
	if(bin=="&&"){
		return 3;
	}
	if(bin=="==" || bin=="!=" || bin=="<" || bin==">" || bin=="<=" || bin==">="){
		return 4;
	}
	if(bin=="+" || bin=="-"){
		return 5;
	}
	if(bin=="*" || bin=="/" || bin=="%"){
		return 6;
	}
	return 0;
}
bool Code::IsRightAssociativeBinOperator(string bin){
	return bin=="=" || bin=="+=" || bin=="-=" ||
	bin=="*=" || bin=="/=" || bin=="%=";
}
bool Code::BinOrder(string binl,string binr){
	// true: binl<binr
	// false: binl>binr
	if(GetBinOperandPriority(binl)>GetBinOperandPriority(binr)){
		return true;
	}
	if(GetBinOperandPriority(binl)<GetBinOperandPriority(binr)){
		return false;
	}
	return !IsRightAssociativeBinOperator(binl);
}
shared_ptr<Expression> Code::BuildExpression(vector<shared_ptr<Expression>> &ops,
vector<string> &bins){
	stack<pair<shared_ptr<Expression>,string>> left;
	stack<pair<string,shared_ptr<Expression>>> right;
	shared_ptr<Expression> operand=ops[0];
	for(int i=ops.size()-1;i>=1;i--){
		right.push(pair<string,shared_ptr<Expression>>(bins[i-1],ops[i]));
	}
	shared_ptr<Expression> opL,opR;
	string bin;
	for(;;){
    	while(!left.empty() and (right.empty() or BinOrder(left.top().second,right.top().first))) {
        	opL = left.top().first;
        	bin = left.top().second;
        	left.pop();
        	opR = operand;
        	shared_ptr<BinOperator> calc=make_shared<BinOperator>();
			calc->left=opL;
			calc->right=opR;
			calc->operation=bin;
			operand=calc;
			
	    }
	    if (right.empty()){
			break;
		}
	    shared_ptr<Expression> temp = right.top().second;
	    bin = right.top().first;
	    right.pop();
	    left.push(pair<shared_ptr<Expression>,string>(operand, bin));
	    operand = temp;
	}
	return operand;
}
bool Code::IsBinOperator(string str){
	vector<string> values{
		"+","-","*","/","%","=","<",">","==","!=",
		"<=",">=","+=","-=","*=","/=","%=","&&","||"
	};
	return find(values.begin(),values.end(),str)!=values.end();
}
bool Code::IsLeftUnaryOperator(string str){
	vector<const char*> values{
		"+","-","++","--","!"
	};
	return find(values.begin(),values.end(),str)!=values.end();
}
bool Code::IsRightUnaryOperator(string str){
	vector<const char*> values{
		"++","--"
	};
	return find(values.begin(),values.end(),str)!=values.end();
}
