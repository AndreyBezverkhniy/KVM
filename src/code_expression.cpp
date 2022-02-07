#include "code_expression.h"
#include "utils.h"
#include <stack>
#include "expression.h"

bool ReadExpression(const vector<Literal> &vec,int &index,shared_ptr<Expression> &expression){
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
bool ReadBinOperator(const vector<Literal> &vec,int &index,string &bin){
	// + - * / % = < > == != <= >= += -= *= /= %= && ||
	vector<const char*> operators{
		"+","-","*","/","%","=","<",">","==","!=",
		"<=",">=","+=","-=","*=","/=","%=","&&","||"
	};
	if(vec[index].type!=SIGN){
		return false;
	}
	for(int i=0;i<operators.size();i++){
		if(vec[index].str==(string)operators[i]){
			bin=vec[index].str;
			index=index+1;
			return true;
		}
	}
	return false;
}
bool ReadOperand(const vector<Literal> &vec,int &index,shared_ptr<Expression> &operand){
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
bool ReadLeftUnaryOperator(const vector<Literal> &vec,int &index,string &lunar){
	// ! ++ -- + -
	vector<const char*> operators{
		"+","-","++","--","!"
	};
	if(vec[index].type!=SIGN){
		return false;
	}
	for(int i=0;i<operators.size();i++){
		if(vec[index].str==(string)operators[i]){
			lunar=vec[index].str;
			index=index+1;
			return true;
		}
	}
	return false;
}
bool ReadRightUnaryOperator(const vector<Literal> &vec,int &index,string &runar){
	// ++ --
	vector<const char*> operators{
		"++","--"
	};
	if(vec[index].type!=SIGN){
		return false;
	}
	for(int i=0;i<operators.size();i++){
		if(vec[index].str==(string)operators[i]){
			runar=vec[index].str;
			index=index+1;
			return true;
		}
	}
	return false;
}
bool ReadSimpleExpression(const vector<Literal> &vec,int &index,
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
bool ReadParenthesizedExpression(const vector<Literal> &vec,int &index,
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
bool ReadNumber(const vector<Literal> &vec,int &index,shared_ptr<Number> &number){
	if(!IsNumber(vec[index].str)){
		return false;
	}
	number=make_shared<Number>(ToInt(vec[index].str));
	index=index+1;
	return true;
}
bool ReadFunctionCall(const vector<Literal> &vec,int &index,shared_ptr<FunctionCall> &fcall){
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
	ptr->signature.arguments.resize(ptr->arguments.size());
	fcall=ptr;
	index=new_index;
	return true;
}
bool ReadVariableName(const vector<Literal> &vec,int &index,shared_ptr<VariableName> &variable){
	if(!IsIdentifier(vec[index].str)){
		return false;
	}
	variable=make_shared<VariableName>(vec[index].str);
	index=index+1;
	return true;
}
int GetBinOperandPriority(string bin){
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
bool IsRightAssociativeBinOperator(string bin){
	return bin=="=" || bin=="+=" || bin=="-=" ||
	bin=="*=" || bin=="/=" || bin=="%=";
}
bool BinOrder(string binl,string binr){
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
shared_ptr<Expression> BuildExpression(vector<shared_ptr<Expression>> &ops,vector<string> &bins){
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
