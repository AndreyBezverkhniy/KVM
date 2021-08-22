#pragma once

#include <string>

using namespace std;

// Literal can be empty or typed
const char EMPTY_LITERAL = 0; // with no value
const char SPACES_LITERAL = 1; // ' ', '\t', '\n' and '\r' characters
const char WORD_LITERAL = 2;
const char STRING_LITERAL = 3;
const char SIGN_LITERAL = 4;
const char INLINE_COMMENT_LITERAL = 5;
const char MULTILINE_COMMENT_LITERAL = 6;

// literal can be build character by character
// start state is "in progress", that changes to "complited" or "failed"
const char IN_PROGRESS_LITERAL_STATE = 0;
const char COMPLITED_LITERAL_STATE = 1;
const char FAILED_LITERAL_STATE = 2;

class Literal{

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
		|| character >= 'A' && character <= 'Z') {
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
		state = COMPLITED_LITERAL_STATE; // spaces ends
		return false;
	}

	bool addSymbolToWord(char character) {
		if (character >= '0' && character <= '9'
		|| character >= 'a' && character <= 'z'
		|| character >= 'A' && character <= 'Z') {
			value += character;
			return true;
		}
		state = COMPLITED_LITERAL_STATE; // word ends
		return false;
	}

	bool addSymbolToString(char character) {
		if (character == '"') {
			state = COMPLITED_LITERAL_STATE; // string ends
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
			state = COMPLITED_LITERAL_STATE;
			return true;
		}
		state = COMPLITED_LITERAL_STATE;
		return false;
	}

	bool addSymbolToInlineComment(char character) {
		if (character == '\n') {
			state = COMPLITED_LITERAL_STATE;
			return false;
		}
		value += character;
		return true;
	}
	
	bool addSymbolToMultilineComment(char character) {
		if (character == '/' && value.size() > 0
		&& value[value.size()-1] == '*') {
			value.pop_back(); // delete '*' character
			state = COMPLITED_LITERAL_STATE;
		} else {
			value += character;
		}
		return true;
	}

	void makeComplitedEmpty() {
		// can't make empty a complited literal
		return;
	}

	void makeComplitedSpaces() {
		state = COMPLITED_LITERAL_STATE; // just consider it to be
	}

	void makeComplitedWord() {
		state = COMPLITED_LITERAL_STATE; // just consider it to be
	}

	void makeComplitedString() {
		// state complited occurs explicitely when '"' is added
		// otherwise string is not ended by '"' that means fail
		state = FAILED_LITERAL_STATE;
	}

	void makeComplitedSign() {
		state = COMPLITED_LITERAL_STATE; // just consider it to be
	}

	void makeComplitedInlineComment() {
		state = COMPLITED_LITERAL_STATE; // just consider it to be
	}

	void makeComplitedMultilineComment() {
		// state complited occurs explicitely when "*/" is added
		// otherwise multiline comment is not ended by "*/" that means fail
		state = FAILED_LITERAL_STATE;
	}

	public:

	Literal() {
		makeEmpty();
	}

	Literal(string _value, char _type) {
		value = _value;
		type = _type;
		// user literal instance considered to be complited
		state = COMPLITED_LITERAL_STATE;
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

	bool isComplited() const {
		return state == COMPLITED_LITERAL_STATE;
	}

	bool isFailed() const {
		return state == FAILED_LITERAL_STATE;
	}

	bool isInProgress() const {
		return state == IN_PROGRESS_LITERAL_STATE;
	}

	bool addNextSymbol(char character) {
		if (state != IN_PROGRESS_LITERAL_STATE) {
			// No way to expand complited or failed literal
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

	void makeComplited() {
		if (state != IN_PROGRESS_LITERAL_STATE) {
			  return;
		}
		switch (type) {
		case EMPTY_LITERAL:
			return makeComplitedEmpty();
		case SPACES_LITERAL:
			return makeComplitedSpaces();
		case WORD_LITERAL:
			return makeComplitedWord();
		case STRING_LITERAL:
			return makeComplitedString();
		case SIGN_LITERAL:
			return makeComplitedSign();
		case INLINE_COMMENT_LITERAL:
			return makeComplitedInlineComment();
		case MULTILINE_COMMENT_LITERAL:
			return makeComplitedMultilineComment();
		};
	}

	char* getTypeName() const {
		switch (type) {
		case EMPTY_LITERAL:
			return "EMPTY";
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
		}
	}

	friend bool operator==(Literal l,Literal r) {
		// value univocal defines type
		// state does not matter
		// comparation of content only
		return l.value == r.value;
	}

	friend bool operator!=(Literal l,Literal r) {
		return !(l == r);
	}

};
