#include "read_program.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "memory.hpp"
#include "literal.hpp"
#include "function.hpp"
#include "utils.hpp"
#include "errors.hpp"

using namespace std;

bool readSourceFile(vector<string> &importChain, string sourceFilePath);

bool parseArgumentList(int &literalIndex, vector<string> &argumentList) {
	
	// check arguments presence
	if (program[literalIndex] != Literal(")")) {

		string argumentName;
		for (;;) {

			if (program[literalIndex].getType() != WORD_LITERAL) {
				showErrorMessage(errorIncorrectArgument, argumentName);
				return false;
			}
			argumentName = program[literalIndex].getValue();
			if (hasVectorAnElement(argumentList, argumentName)) {
				showErrorMessage(errorRepeatedArgument, argumentName);
				return false;
			}
			if (!isUserDefinedNamePermitted(argumentName)) {
				showErrorMessage(errorUserDefinedName, argumentName);
				return false;
			}
			argumentList.push_back(argumentName);
			literalIndex++; // argumentName

			if (!parseExactLiteral(literalIndex, ",", false)) { // ','
				break; // no more arguments
			}

		}

	}

	return true;

}

// second iteration of program read
// collect information about functions declared
bool prepareFunctions(int &failureIntex) {

	int &literalIndex=failureIntex;
	literalIndex=0;

	while (program[literalIndex] != literalEOF) {

		// function
		if (program[literalIndex] != Literal("function")) {
			literalIndex++; // farther search for function declaration
			continue;
		}
		literalIndex++;
		// functionName
		string functionName = program[literalIndex].getValue();
		if (program[literalIndex].getType() != WORD_LITERAL) {
			showErrorMessage(errorFunctionName, functionName);
			return false;
		}
		if (!isUserDefinedNamePermitted(functionName)) {
			showErrorMessage(errorUserDefinedName, functionName);
			return false;
		}
		if (doesFunctionExist(functionName)) {
			showErrorMessage(errorRepeatedFunction, functionName);
			return false;
		}
		literalIndex++;
		// '('
		if (!parseExactLiteral(literalIndex, "(")) {
			showErrorMessage(errorFunctionDeclarationPattern, "'('");
			return false;
		}
		// arguments
		FunctionDescription &functionDescription = functions[functionName];
		if (!parseArgumentList(literalIndex,functionDescription.arguments)) {
			return false;
		}
		// ')'
		if (!parseExactLiteral(literalIndex, ")")) {
			showErrorMessage(errorFunctionDeclarationPattern, "')'");
			return false;
		}
		// '{'
		if (!parseExactLiteral(literalIndex, "{")) {
			showErrorMessage(errorFunctionDeclarationPattern, "'{'");
			return false;
		}
		// save function start point
		functionDescription.bodyIntex = literalIndex;
		skipCurrentBlock(literalIndex); // skip function body
		// '}'
		if (!parseExactLiteral(literalIndex, "}")) {
			showErrorMessage(errorFunctionDeclarationPattern, "'}'");
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
		showErrorMessage(errorRecursiveImport, importingFilePath);
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
		showErrorMessage(errorReadFile, sourceFilePath);
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
