#pragma once
#include <string>

using namespace std;

enum LexemeStatus{
	NOT_COMPLITED,
	COMPLITED,
	EXTENDABLE
};

enum LexemeType{
	INCORRECT,
	EMPTY,
	SPACE,
	ONELINE_COMMENT,
	MULTILINE_COMMENT,
	SIGN,
	WORD,
	STRING
};

enum LexemeState{
	INIT,
	END, // last for all complited
	SLASH,
	SLASH_STAR_X,
	SLASH_STAR_X_STAR,
	SLASH_SLASH_X,
	SPACERS,
	ALPHADIGIT,
	QUOTATION_X,
	AND, // &
	OR, // |
	PLUS, // +
	MINUS, // -
	PRE_EQUAL // %*<>=!
};

class Lexeme{
private:
	LexemeState state;
	LexemeStatus status;
public:
	string str;
	LexemeType type;
	Lexeme();
	Lexeme(string str);
	Lexeme(char* str);
	void Clean();
	bool AddChar(char ch);
	bool IsEmpty() const;
	bool IsReady() const;
	string ToString() const;
	friend bool operator==(Lexeme l,Lexeme r);
	friend bool operator!=(Lexeme l,Lexeme r);
};
