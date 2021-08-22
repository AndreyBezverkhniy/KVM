#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "literal.hpp"

using namespace std;

bool readSourceFile(vector<string> &importChain, string sourceFilePath,
vector<Literal> &program);

// returns true if importingFilePath is in import chain,
// i.e. imports it-self through several imports
bool checkRecursiveImport(vector<string> &importChain,
string importingFilePath) {
	auto iter =
	std::find(importChain.begin(), importChain.end(), importingFilePath);
	return iter != importChain.end();
}

void popNLastElements(vector<Literal> &program, int n) {
	for (int i = 0; i < n; i ++) {
		program.pop_back();
	}
}

bool expandImportInstruction(vector<string> &importChain, vector<Literal> &program) {
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
	popNLastElements(program,3); // delete import instruction
	return readSourceFile(importChain, importingFilePath, program);
}

// check if program ends by import instruction
bool endsByImportInstruction(const vector<Literal> &program) {
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
bool readSourceFile(vector<string> &importChain, string sourceFilePath,
vector<Literal> &program) {

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
			if (endsByImportInstruction(program)) {
				bool importedSuccessfully =
				expandImportInstruction(importChain, program);
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

// it's just shell for readSourceFile function to create variable
// importChain for passing to the first argument of reference type
bool readProgram(vector<Literal> &program, string sourceFilePath) {
	vector<string> importChain;
	// chain of nested file imports for recursive imports tracing
	// root source file considered to be the first member of the chain
	return readSourceFile(importChain, sourceFilePath, program);
}
