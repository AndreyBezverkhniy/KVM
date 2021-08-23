#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "program memory.hpp"
#include "literal.hpp"
#include "program function.hpp"

using namespace std;

template<class T> bool hasVectorAnElement(vector<T> &array, T element) {
	return find(array.begin(), array.end(), element) != array.end();
}

void skipCurrentBlock(int &literalIntex) {
	int blockDeep = 1; // deep of nesting blocks
	while (literalIntex < program.size()) {
		if (program[literalIntex] == Literal("{", SIGN_LITERAL)) {
			blockDeep++;
		}
		if (program[literalIntex] == Literal("}", SIGN_LITERAL)) {
			blockDeep--;
		}
		if (blockDeep == 0) {
			// whole block (with nested ones) is skiped
			break;
		}
		literalIntex++;
	}
}

bool parseArgumentList(int &literalIntex, vector<string> &argumentList) {
	
	if (program[literalIntex] != Literal(")", SIGN_LITERAL)) {
		string argumentName;
		for (;;) {
			if (program[literalIntex].getType() != WORD_LITERAL) {
				cout << argumentName << " is not an agrument name" << endl;
				return false;
			}
			argumentName = program[literalIntex].getValue();
			if (hasVectorAnElement(argumentList, argumentName)) {
				cout << "Repeated name of function argument "
				<< argumentName << endl;
				return false;
			}
			argumentList.push_back(argumentName);
			literalIntex++; // argumentName
			if (program[literalIntex] != Literal(",", SIGN_LITERAL)) {
				break; // no more arguments
			}
			literalIntex++; // ','
		}
	}
	return true;

}

// second iteration of program read
// collect information about functions declared
bool prepareFunctions(int &failureIntex) {
	int &literalIntex=failureIntex;
	literalIntex=0;
	while (literalIntex < program.size()) {

		if (program[literalIntex] != Literal("function")) {
			// search function declaration
			literalIntex++;
			continue;
		}
		literalIntex++; // function

		string functionName = program[literalIntex].getValue();
		if (program[literalIntex].getType() != WORD_LITERAL) {
			cout << functionName << " is not a function name" << endl;
			return false;
		}
		if (doesFunctionExist(functionName)) {
			cout << "Repeated declaration of function " << functionName << endl;
			return false;
		}
		literalIntex++; // functionName

		if (program[literalIntex] != Literal("(")) {
			cout << "Expected '(' after function name" << endl;
			return false;
		}
		literalIntex++; // '('

		FunctionDescription &functionDescription = functions[functionName];

		parseArgumentList(literalIntex,functionDescription.arguments);

		if (program[literalIntex] != Literal(")", SIGN_LITERAL)) {
			cout << "Expected ')' after function name(<arguments" << endl;
			return false;
		}
		literalIntex++; // ')'

		if (program[literalIntex] != Literal("{", SIGN_LITERAL)) {
			cout << "Expected '{' after function name(<arguments>)" << endl;
			return false;
		}
		literalIntex++; // '{'

		// save function start point
		functionDescription.bodyIntex = literalIntex;
		skipCurrentBlock(literalIntex); // skip function body

		if (program[literalIntex] != Literal("}", SIGN_LITERAL)) {
			cout << "Expected '}' at the end of function " << functionName
			<< endl;
			return false;
		}
		literalIntex++; // '}'
	}
	return true;
}

bool readSourceFile(vector<string> &importChain, string sourceFilePath);

// returns true if importingFilePath is in import chain,
// i.e. imports it-self through several imports
bool checkRecursiveImport(vector<string> &importChain,
string importingFilePath) {
	auto iter =
	std::find(importChain.begin(), importChain.end(), importingFilePath);
	return iter != importChain.end();
}

void popNLastLiterals(int n) {
	for (int i = 0; i < n; i ++) {
		program.pop_back();
	}
}

bool expandImportInstruction(vector<string> &importChain) {
	string importingFilePath; // file to import
	importingFilePath = program[program.size()-2].getValue();
	if (checkRecursiveImport(importChain, importingFilePath)) {
		cout << "Recursive import of " << importingFilePath << endl;
		for (auto iter =
		std::find(importChain.begin(), importChain.end(), importingFilePath);
		iter != importChain.end(); iter++) {
			cout << *iter << " >> " << endl;
		}
		cout << importingFilePath << endl;
		return false;
	}
	popNLastLiterals(3); // delete import instruction
	return readSourceFile(importChain, importingFilePath);
}

// check if program ends by import instruction
bool endsByImportInstruction() {
	// import instruction consists of
	// WORD(import) STRING(srcFilePath) SIGN(;)
	int size = program.size();
	if (size < 3) {
		return false;
	}
	return (program[size - 3] == Literal("import", WORD_LITERAL) &&
	program[size - 2].getType() == STRING_LITERAL &&
	program[size - 1] == Literal(";", SIGN_LITERAL));
}

void calculateNextCursorPosition(int &line, int &column, char character) {
	column++;
	if (character == '\n') {
		line++;
		column=1;
	}
}	

void closeFileAndPopFromImportChain(FILE *fp, vector<string> &importChain){
	fclose(fp);
	importChain.pop_back();
}

// reads program farther from source file
// importChain is order of nested imports, includes the root source file
// returns true if success
bool readSourceFile(vector<string> &importChain, string sourceFilePath) {

	FILE *fp;
	fp = fopen(sourceFilePath.c_str(), "rb");
	if (!fp) {
		cout << "Can't open soure file: " << sourceFilePath << endl;
		return false;
	}

	// add path to chain of nested imports
	importChain.push_back(sourceFilePath);

	Literal literal; // base brick of program content
	int byte; // for byte by byte file reading
	int line = 1;    // cursor position
	int column = 1;  // in current file

	// main circle of file reading
	do {
		byte = fgetc(fp);
		bool isSymbolAddedToLiteral;
		if (byte == EOF) {
			literal.makeComplited(); // try to end last literal
			isSymbolAddedToLiteral = false; // can't add EOF to literal
		} else {
			isSymbolAddedToLiteral = literal.addNextSymbol(byte);
		}
		if (literal.isFailed()) {
			// literal pattern ruined
			cout << "Invalid literal at " << sourceFilePath << ": "
				<< line << ":" << column << endl;
			closeFileAndPopFromImportChain(fp,importChain);
			return false;
		}
		if (!isSymbolAddedToLiteral && literal.isComplited()) {
			// the case when symbol means current literal has been complited
			if (literal.getType() != EMPTY_LITERAL
			&& literal.getType() != SPACES_LITERAL
			&& literal.getType() != INLINE_COMMENT_LITERAL
			&& literal.getType() != MULTILINE_COMMENT_LITERAL) {
				program.push_back(literal); // save finished literal
			}
			literal.makeEmpty();           // initiate next literal
			literal.addNextSymbol(byte);   // by symbol
			// handle import instruction if presents
			if (endsByImportInstruction()) {
				bool importedSuccessfully =
				expandImportInstruction(importChain);
				if (!importedSuccessfully) {
					// expanding import failed
					closeFileAndPopFromImportChain(fp, importChain);
					return false;
				}
			}
		}
		// update cursor position
		calculateNextCursorPosition(line, column, byte);
	} while (byte != EOF);

	closeFileAndPopFromImportChain(fp,importChain);
	return true;

}

// gets program from sourceFilePath and imported ones
// collects information about function declarations in program
bool readProgram(string sourceFilePath) {

	vector<string> importChain;
	// chain of nested file imports for recursive imports tracing
	// root source file considered to be the first member of the chain
	program.clear();
	if (!readSourceFile(importChain, sourceFilePath)) {
		return false;
	}

	int failureIndex; // literal index where function preparation failed
	if (!prepareFunctions(failureIndex)) {
		cout << "Functions preparation failed at literal index "
		<< failureIndex << endl;
		return false;
	}

	return true;

}
