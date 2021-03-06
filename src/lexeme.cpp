#include "lexeme.h"

Lexeme::Lexeme() {
	Clean();
}
Lexeme::Lexeme(string str){
	this->str=str;
}
Lexeme::Lexeme(char* str){
	this->str=string(str);
}
void Lexeme::SetFilePositionLexeme(const string * str,int line,int column){
	this->module=str;
	this->line=line;
	this->column=column;
}
const string& Lexeme::GetFile() const {
	return *module;
}
int Lexeme::GetLine() const {
	return line;
}
int Lexeme::GetColumn() const {
	return column;
}
void Lexeme::Clean(){
	type=EMPTY;
	str="";
	status=NOT_COMPLITED;
	state=INIT;
}
bool Lexeme::AddChar(char ch){
	switch(state){
		case INIT:
		{
			if(ch=='/'){
				type=SIGN;
				state=SLASH;
				status=EXTENDABLE;
				str+=ch;
			} else if(ch==' ' || ch=='\t' || ch=='\n') {
				type=SPACE;
				state=SPACERS;
				status=EXTENDABLE;
				str+=ch;
			} else if(isdigit(ch) || isalpha(ch)) {
				type=WORD;
				state=ALPHADIGIT;
				status=EXTENDABLE;
				str+=ch;
			} else if(ch=='"') {
				type=INCORRECT;
				state=QUOTATION_X;
				status=NOT_COMPLITED;
			} else if(ch=='&') {
				type=INCORRECT;
				state=AND;
				status=NOT_COMPLITED;
				str+=ch;
			} else if(ch=='|') {
				type=INCORRECT;
				state=OR;
				status=NOT_COMPLITED;
				str+=ch;
			} else if(ch=='+') {
				type=SIGN;
				state=PLUS;
				status=EXTENDABLE;
				str+=ch;
			} else if(ch=='-') {
				type=SIGN;
				state=MINUS;
				status=EXTENDABLE;
				str+=ch;
			} else if(ch=='%' || ch=='*' || ch=='<' || ch=='>' || ch=='=' ||
			ch=='!') {
				type=SIGN;
				state=PRE_EQUAL;
				status=EXTENDABLE;
				str+=ch;
			} else if(ch=='(' || ch==')' || ch=='{' || ch=='}' || ch==',' ||
			ch==';') {
				type=SIGN;
				state=END;
				status=COMPLITED;
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case SLASH:
		{
			if (ch=='/') {
				type=ONELINE_COMMENT;
				state=SLASH_SLASH_X;
				status=EXTENDABLE;
				str="";
			} else if (ch=='=') {
				type=SIGN;
				state=END;
				status=COMPLITED;
				str+=ch;
			} else if (ch=='*') {
				type=INCORRECT;
				state=SLASH_STAR_X;
				status=NOT_COMPLITED;
				str="";
			} else {
				return false;
			}
			return true;
		}
		case SLASH_STAR_X:
		{
			if (ch=='*') {
				type=INCORRECT;
				state=SLASH_STAR_X_STAR;
				status=NOT_COMPLITED;
			} else {
				str+=ch;
			}
			return true;
		}
		case SLASH_STAR_X_STAR:
		{
			if (ch=='/') {
				type=MULTILINE_COMMENT;
				state=END;
				status=COMPLITED;
			} else if (ch=='*') {
				str+=ch;
			} else {
				type=INCORRECT;
				state=SLASH_STAR_X;
				status=NOT_COMPLITED;
				str+='*';
				str+=ch;
			}
			return true;
		}
		case SLASH_SLASH_X:
		{
			if (ch!='\n') {
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case SPACERS:
		{
			if (ch==' ' || ch=='\t' || ch=='\n') {
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case ALPHADIGIT:
		{
			if(isalpha(ch) || isdigit(ch)){
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case QUOTATION_X:
		{
			if (ch=='"') {
				type=STRING;
				state=END;
				status=COMPLITED;
			} else {
				type=INCORRECT;
				state=QUOTATION_X;
				status=NOT_COMPLITED;
				str+=ch;
			}
			return true;
		}
		case AND:
		{
			if (ch=='&') {
				type=SIGN;
				state=END;
				status=COMPLITED;
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case OR:
		{
			if (ch=='|') {
				type=SIGN;
				state=END;
				status=COMPLITED;
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case PLUS:
		{
			if (ch=='+' || ch=='=') {
				type=SIGN;
				state=END;
				status=COMPLITED;
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case MINUS:
		{
			if (ch=='-' || ch=='=') {
				type=SIGN;
				state=END;
				status=COMPLITED;
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case PRE_EQUAL:
		{
			if (ch=='=') {
				type=SIGN;
				state=END;
				status=COMPLITED;
				str+=ch;
			} else {
				return false;
			}
			return true;
		}
		case END:
			return false;
	}
	return false;
}
bool Lexeme::IsEmpty() const {
	return state==LexemeState::INIT;
}
bool Lexeme::IsReady() const {
	return status==LexemeStatus::COMPLITED ||
	status==LexemeStatus::EXTENDABLE;
}
string Lexeme::ToString() const {
	string result;
	switch(type){
		case EMPTY:
			result+="EMPTY";
			break;
		case INCORRECT:
			result+="INCORRECT";
			break;
		case SPACE:
			result+="SPACE";
			break;
		case ONELINE_COMMENT:
			result+="ONELINE";
			break;
		case MULTILINE_COMMENT:
			result+="MULTILINE";
			break;
		case SIGN:
			result+="SIGN";
			break;
		case WORD:
			result+="WORD";
			break;
		case STRING:
			result+="STRING";
			break;
		default:
			result+="ERROR";
	}
	return result+":{"+str+"}";
}
bool operator==(Lexeme l,Lexeme r){
	return l.str==r.str;
}
bool operator!=(Lexeme l,Lexeme r){
	return !(l==r);
}
