#include "read_program.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "memory.hpp"
#include "literal.hpp"
#include "function.hpp"
#include "utils.hpp"

using namespace std;

bool readSourceFile(vector<string> &importChain, string sourceFilePath);

bool parseArgumentList(int &literalIntex, vector<string> &argumentList) {
	
	// check arguments presence
	if (program[literalIntex] != Literal(")")) {

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

			if (!parseExactLiteral(literalIntex, ",", false)) { // ','
				break; // no more arguments
			}

		}

	}

	return true;

}

// second iteration of program read
// collect information about functions declared
bool prepareFunctions(int &failureIntex) {

	int &literalIntex=failureIntex;
	literalIntex=0;

	while (program[literalIntex] != literalEOF) {

		if (program[literalIntex] != Literal("function")) {
			literalIntex++; // farther search for function declaration
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

		if (!parseExactLiteral(literalIntex, "(")) { // '('
			return false;
		}

		// arguments
		FunctionDescription &functionDescription = functions[functionName];
		parseArgumentList(literalIntex,functionDescription.arguments);

		if (!parseExactLiteral(literalIntex, ")")) { // ')'
			return false;
		}

		if (!parseExactLiteral(literalIntex, "{")) { // '{'
			return false;
		}

		// save function start point
		functionDescription.bodyIntex = literalIntex;
		skipCurrentBlock(literalIntex); // skip function body

		if (!parseExactLiteral(literalIntex, "}")) { // '}'
			return false;
		}
		
	}

	return true;

}

// returns true if importingFilePath is in import chain,
// i.e. imports it-self through several imports
bool checkRecursiveImport(vector<string> &importChain,
string importingFilePath) {
	return hasVectorAnElement(importChain, importingFilePath);
}

void popNLastLiterals(int n) {
	for (int i = 0; i < n; i ++) {
		program.pop_back();
	}
}

// considered that complete import instruction is at the end of
// loaded program piece
bool expandImportInstruction(vector<string> &importChain) {

	string importingFilePath; // file to import
	importingFilePath = program[program.size()-2].getValue();

	// recursive import forbidden
	if (checkRecursiveImport(importChain, importingFilePath)) {
		cout << "Recursive import of " << importingFilePath << endl;
		auto iter =
		std::find(importChain.begin(), importChain.end(), importingFilePath);
		while (iter != importChain.end()) {
			cout << *iter << " >> " << endl;
			iter++;
		}
		cout << importingFilePath << endl;
		return false;
	}

	popNLastLiterals(3); // delete import instruction: 1)import 2)src 3)';'

	return readSourceFile(importChain, importingFilePath);

}

// check if loaded piece of program ends by import instruction
bool endsByImportInstruction() {
	// import instruction consists of
	// WORD(import) STRING(srcFilePath) SIGN(;)
	int size = program.size();
	if (size < 3) {
		return false;
	}
	return (program[size - 3] == Literal("import") &&
	program[size - 2].getType() == STRING_LITERAL &&
	program[size - 1] == Literal(";"));
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
			literal.makeCompleted(); // try to end last literal
			isSymbolAddedToLiteral = false; // ignore EOF character, file may
			// be imported
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

		if (literal.isCompleted()) {
			if (literal.getType() != EMPTY_LITERAL
			&& literal.getType() != SPACES_LITERAL
			&& literal.getType() != INLINE_COMMENT_LITERAL
			&& literal.getType() != MULTILINE_COMMENT_LITERAL) {
				program.push_back(literal); // save completed literal
				// handle import instruction if presents
				if (endsByImportInstruction()) {
					bool importedSuccessfully =
					expandImportInstruction(importChain);
					if (!importedSuccessfully) {
						closeFileAndPopFromImportChain(fp, importChain);
						return false;
					}
				}
			}
			literal.makeEmpty();
		}

		if(!isSymbolAddedToLiteral) {
			// when symbol starts next literal, attempt to add symbol to
			// current literal fails and make this literal completed or failed
			// we have to add symbol to next literal if completed
			literal.addNextSymbol(byte);
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
	// chain of nested file imports for recursive imports tracing.
	// root source file considered to be the first member of the chain.
	program.clear();
	if (!readSourceFile(importChain, sourceFilePath)) {
		return false;
	}
	program.push_back(literalEOF);
	// literalEOF prevent error "out of range"
	// in checks like program[index] == notEOFliteral

	int failureIndex; // literal index where function preparation failed
	if (!prepareFunctions(failureIndex)) {
		cout << "Functions preparation failed at literal index: "
		<< failureIndex << endl;
		for (int i=0;i<failureIndex;i++) {
			cout << program[i].getValue() << " ";
		}
		cout <<endl;
		return false;
	}

	return true;

}
