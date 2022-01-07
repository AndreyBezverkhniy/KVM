#include <string>

using namespace std;

typedef char LTYPE;
typedef char LSTATE;

const LSTATE STATE_COMPLITED = 0;
const LSTATE STATE_NOT_COMPLITED = 1;
const LSTATE STATE_EXTENDABLE = 2;

const LTYPE TYPE_SPACE = 0;
const LTYPE TYPE_SIGN = 1;
const LTYPE TYPE_WORD = 2;
const LTYPE TYPE_STRING = 3;
const LTYPE TYPE_ONELINE_COMMENT = 4;
const LTYPE TYPE_MULTILINE_COMMENT = 5;

class Literal{
private:

	string value;
	LTYPE type;
	LSTATE state;

public:

	Literal() : state(STATE_EMPTY_LITERAL) {}

	bool isEmpty() {
		return state == STATE_EMPTY_LITERAL;
	}
	bool isNotComplited() {
		return state == STATE_NOT_COMPLITED;
	}
	bool isReady() {
		return state == STATE_READY;
	}
	LSTATE getState() {
		return state;
	}

	bool isSpace() {
		return type == TYPE_SPACE;
	}
	bool isSign() {
		return type == TYPE_SIGN;
	}
	bool isWord() {
		return type == TYPE_WORD;
	}
	bool isString() {
		return type == TYPE_STRING;
	}
	bool isOnelineComment() {
		return type == TYPE_ONELINE_COMMENT;
	}
	bool isMultilineComment() {
		return TYPE_MULTILINE_COMMENT;
	}
	LTYPE getType() {
		return type;
	}

	const string& getValue() {
		return value;
	}

	friend bool operator==(Literal l, Literal r) {
		return l.type == r.type && l.value == r.value;
	}

	LSTATE addCharacter(char ch) {

		return state;
	}

};
