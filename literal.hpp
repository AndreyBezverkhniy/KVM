#pragma once

#include <string>

using namespace std;

// Literal can be empty or typed
const char EMPTY_LITERAL = 0; // with no value
const char SPACES_LITERAL = 1; // ' ', '\t', '\n' and '\r' characters
const char WORD_LITERAL = 2; // consists digits, alphabet characters and '_'
const char STRING_LITERAL = 3; // "..."
const char SIGN_LITERAL = 4;
const char INLINE_COMMENT_LITERAL = 5; // //...\n
const char MULTILINE_COMMENT_LITERAL = 6; // /* ...*/
const char EOF_LITERAL = 7; // added for detecting end of program without
// check of vector size. artificial constructing only

// literal can be build character by character
// start state is "in progress", that changes to "completed" or "failed"
const char IN_PROGRESS_LITERAL_STATE = 0;
const char COMPLETED_LITERAL_STATE = 1;
const char FAILED_LITERAL_STATE = 2;

class Literal{

	public:

	Literal() {
		makeEmpty();
	}

	Literal(string _value) {
		makeEmpty();
		for (char symbol : _value) {
			addNextSymbol(symbol);
		}
		makeCompleted();
	}

	Literal(string _value, int _type) {
		// user specified literal
		value = _value;
		type = _type;
		state = COMPLETED_LITERAL_STATE;
	}

	void makeEmpty() {
		value.clear();
		type = EMPTY_LITERAL;
		// waiting of adding characters
		state = IN_PROGRESS_LITERAL_STATE;
	}

	string getValue() const {
		return value;
	}

	int getType() const {
		return type;
	}

	bool isEmpty() const {
		return type == EMPTY_LITERAL;
	}

	int getState() const {
		return state;
	}

	bool isCompleted() const {
		return state == COMPLETED_LITERAL_STATE;
	}

	bool isFailed() const {
		return state == FAILED_LITERAL_STATE;
	}

	bool isInProgress() const {
		return state == IN_PROGRESS_LITERAL_STATE;
	}

	bool addNextSymbol(char character) {
		if (state != IN_PROGRESS_LITERAL_STATE) {
			// No way to expand completed or failed literal
			return false;
		}
		switch (type) {
		case EMPTY_LITERAL:
			return addSymbolToEmpty(character);
		case SPACES_LITERAL:
			return addSymbolToSpaces(character);
		case WORD_LITERAL:
			return addSymbolToWord(character);
		case STRING_LITERAL:
			return addSymbolToString(character);
		case SIGN_LITERAL:
			return addSymbolToSign(character);
		case INLINE_COMMENT_LITERAL:
			return addSymbolToInlineComment(character);
		case MULTILINE_COMMENT_LITERAL:
			return addSymbolToMultilineComment(character);
		};
		return false;
	}

	void makeCompleted() {
		if (state != IN_PROGRESS_LITERAL_STATE) {
			  return;
		}
		switch (type) {
		case EMPTY_LITERAL:
			return makeCompletedEmpty();
		case SPACES_LITERAL:
			return makeCompletedSpaces();
		case WORD_LITERAL:
			return makeCompletedWord();
		case STRING_LITERAL:
			return makeCompletedString();
		case SIGN_LITERAL:
			return makeCompletedSign();
		case INLINE_COMMENT_LITERAL:
			return makeCompletedInlineComment();
		case MULTILINE_COMMENT_LITERAL:
			return makeCompletedMultilineComment();
		};
	}

	string getTypeName() const {
		switch (type) {
		case EMPTY_LITERAL:
			return "EMPTY";
			break;
		case SPACES_LITERAL:
			return "SPACES";
			break;
		case WORD_LITERAL:
			return "WORD";
			break;
		case STRING_LITERAL:
			return "STRING";
			break;
		case SIGN_LITERAL:
			return "SIGN";
			break;
		case INLINE_COMMENT_LITERAL:
			return "INLINE_COMMENT";
			break;
		case MULTILINE_COMMENT_LITERAL:
			return "MULTILINE_COMMENT";
			break;
		case EOF_LITERAL:
			return "EOF";
			break;
		}
	}

	friend bool operator==(Literal l,Literal r) {
		// state does not matter
		return l.type == r.type && l.value == r.value;
	}

	friend bool operator!=(Literal l,Literal r) {
		return !(l == r);
	}

	private:

	char type; // **_LITERAL
	string value;
	char state; // **_LITERAL_state

	bool addSymbolToEmpty(char character) {
		if (character == ' ' || character == '\t' || character == '\r'
		|| character == '\n') {
			type = SPACES_LITERAL;
			value = character;
		} else if (character >= '0' && character <= '9'
		|| character >= 'a' && character <= 'z'
		|| character >= 'A' && character <= 'Z'
		|| character == '_') {
			type = WORD_LITERAL;
			value = character;
		} else if (character == '"') {
			type = STRING_LITERAL;
			value = "";
		} else {
			type = SIGN_LITERAL; // all other symbols start signs
			value = character;
		}
		return true;
	}

	bool addSymbolToSpaces(char character) {
		if (character == ' ' || character == '\t' || character == '\r'
		|| character == '\n') {
			value += character;
			return true;
		}
		state = COMPLETED_LITERAL_STATE; // spaces ends
		return false;
	}

	bool addSymbolToWord(char character) {
		if (character >= '0' && character <= '9'
		|| character >= 'a' && character <= 'z'
		|| character >= 'A' && character <= 'Z'
		|| character == '_') {
			value += character;
			return true;
		}
		state = COMPLETED_LITERAL_STATE; // word ends
		return false;
	}

	bool addSymbolToString(char character) {
		if (character == '"') {
			state = COMPLETED_LITERAL_STATE; // string ends
		} else {
			value += character;
		}
		return true;
	}

	bool addSymbolToSign(char character) {
		if (value == "/" && character == '/') {
			value.clear();
			type = INLINE_COMMENT_LITERAL;
			return true;
		}
		if (value == "/" && character == '*') {
			value.clear();
			type = MULTILINE_COMMENT_LITERAL;
			return true;
		}
		if ( ((value == "=" || value == "!" || value == ">" || value == "<")
		&& character == '=') || (value == "|" && character == '|') ||
		(value == "&" && character == '&') ) {
			value += character;
			state = COMPLETED_LITERAL_STATE;
			return true;
		}
		state = COMPLETED_LITERAL_STATE;
		return false;
	}

	bool addSymbolToInlineComment(char character) {
		if (character == '\n') {
			state = COMPLETED_LITERAL_STATE;
			return false;
		}
		value += character;
		return true;
	}
	
	bool addSymbolToMultilineComment(char character) {
		if (character == '/' && value.size() > 0
		&& value[value.size()-1] == '*') {
			value.pop_back(); // delete '*' character
			state = COMPLETED_LITERAL_STATE;
		} else {
			value += character;
		}
		return true;
	}

	void makeCompletedEmpty() {
		// can't make empty a completed literal
		state = FAILED_LITERAL_STATE;
	}

	void makeCompletedSpaces() {
		state = COMPLETED_LITERAL_STATE; // just consider it to be
	}

	void makeCompletedWord() {
		state = COMPLETED_LITERAL_STATE; // just consider it to be
	}

	void makeCompletedString() {
		// state completed occurs explicitely when '"' is added
		// otherwise string is not ended by '"' that means fail
		state = FAILED_LITERAL_STATE;
	}

	void makeCompletedSign() {
		state = COMPLETED_LITERAL_STATE; // just consider it to be
	}

	void makeCompletedInlineComment() {
		state = COMPLETED_LITERAL_STATE; // just consider it to be
	}

	void makeCompletedMultilineComment() {
		// state completed occurs explicitely when "*/" is added
		// otherwise multiline comment is not ended by "*/" that means fail
		state = FAILED_LITERAL_STATE;
	}

};

Literal MakeLiteralEOF() {
	return Literal("", EOF_LITERAL);
}

const Literal literalEOF = MakeLiteralEOF();
