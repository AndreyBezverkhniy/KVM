/*
KVM is interpretator of my own language Kirpitch
receives exact one argument: path to root source file of Kirpitch project
reads whole program and executes, if it all is possible.
*/

#include <iostream>
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
	if (!readProgram(program, sourceFilePath)) {
		return 0; // reading failed
	}
	printProgram(program);

	return 0;

	// execution
	cout << endl << "EXECUTION" << endl << endl;
	bool success = execute(program);
	cout << endl << "EXECUTION " << (success ? "FINISHED" : "FAILED") << endl;

	return 0;
}
