#pragma once
#include <string>

using namespace std;

enum LiteralStatus{
	NOT_COMPLITED,
	COMPLITED,
	EXTENDABLE
};

enum LiteralType{
	INCORRECT,
	EMPTY,
	SPACE,
	ONELINE_COMMENT,
	MULTILINE_COMMENT,
	SIGN,
	WORD,
	STRING
};

enum LiteralState{
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

class Literal{
private:
	LiteralState state;
	LiteralStatus status;
public:
	string str;
	LiteralType type;
	Literal();
	void Clean();
	bool AddChar(char ch);
	bool IsEmpty() const;
	bool IsReady() const;
	string ToString() const;
};
