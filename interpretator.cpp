/*
KVM is interpretator of my own language Kirpitch
receives exact one argument: path to root source file of Kirpitch project
reads whole program and executes, if it all is possible.
*/

#include <iostream>
#include <map>
#include <string>
#include "program.hpp"

using namespace std;

int main(int argc, char* argv[]) {

	// utility recieves 1 argument - source file path
	if (argc != 2) {
		cout << "Usage: kvm source-file-path" << endl;
		return 0;
	}
	string sourceFilePath = argv[1];

	// reading program
	vector<Literal> program;
	if (!readProgram(sourceFilePath, program)) {
		return 0; // reading failed
	}

	if (!prepareFunctions(program)) {
		return 0; // preparing failed
	}

	printProgram(program);

	//printFunctions(program);

	// execution
	cout << endl << "EXECUTION" << endl << endl;
	bool returned;
	int returnValue;
	bool success = execute(program, returnValue, returned);
	cout << endl;
	if (returned) {
		cout << "return " << returnValue << endl;
	}
	cout << "EXECUTION " << (success ? "FINISHED" : "FAILED") << endl;

	return 0;
}
