#include "code_expression.h"
#include "utils.h"
#include <stack>
#include "expression.h"

bool ReadExpression(const vector<Literal> &vec,int &index,shared_ptr<Expression> &expression){
    int new_index=index;
    vector<shared_ptr<Operand>> ops;
    vector<string> bins;
    shared_ptr<Operand> operand;
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
bool ReadOperand(const vector<Literal> &vec,int &index,shared_ptr<Operand> &operand){
	shared_ptr<Number> number;
	int new_index=index;
	if(!ReadNumber(vec,new_index,number)){
		return false;
	}
	operand=number;
	index=new_index;
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
shared_ptr<Expression> BuildExpression(vector<shared_ptr<Operand>> &ops,vector<string> &bins){
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
