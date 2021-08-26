#pragma once

#include <string>

using namespace std;

extern const char EMPTY_LITERAL;
extern const char SPACES_LITERAL;
extern const char WORD_LITERAL;
extern const char STRING_LITERAL;
extern const char SIGN_LITERAL;
extern const char INLINE_COMMENT_LITERAL;
extern const char MULTILINE_COMMENT_LITERAL;
extern const char EOF_LITERAL;

extern const char IN_PROGRESS_LITERAL_STATE;
extern const char COMPLETED_LITERAL_STATE;
extern const char FAILED_LITERAL_STATE;

class Literal{

	public:

	Literal();
	Literal(string _value);
	Literal(string _value, int _type);
	void makeEmpty();
	string getValue() const;
	int getType() const;
	bool isEmpty() const;
	int getState() const;
	bool isCompleted() const;
	bool isFailed() const;
	bool isInProgress() const;
	bool addNextSymbol(char character);
	void makeCompleted();
	string getTypeName() const;
	friend bool operator==(Literal l,Literal r);
	friend bool operator!=(Literal l,Literal r);

	private:

	char type; // **_LITERAL
	string value;
	char state; // **_LITERAL_state

	bool addSymbolToEmpty(char character);
	bool addSymbolToSpaces(char character);
	bool addSymbolToWord(char character);
	bool addSymbolToString(char character);
	bool addSymbolToSign(char character);
	bool addSymbolToInlineComment(char character);
	bool addSymbolToMultilineComment(char character);
	void makeCompletedEmpty();
	void makeCompletedSpaces();
	void makeCompletedWord();
	void makeCompletedString();
	void makeCompletedSign();
	void makeCompletedInlineComment();
	void makeCompletedMultilineComment();

};

Literal MakeLiteralEOF();

const Literal literalEOF = MakeLiteralEOF();
