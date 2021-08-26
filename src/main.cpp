/*
KVM is interpretator of my own language Kirpitch
receives exact one argument: path to root source file of Kirpitch project
reads whole program and executes, if it all is possible.
*/

#include <iostream>
#include <map>
#include <string>
#include "read_program.hpp"
#include "print_program.hpp"
#include "execute_program.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	// utility recieves 1 argument - source file path
	if (argc != 2) {
		cout << "Usage: kvm source-file-path" << endl;
		return 0;
	}
	string sourceFilePath = argv[1];

	if (!readProgram(sourceFilePath)) {
		return 0; // reading failed
	}
	printProgram();
	executeProgram();

	return 0;
}
