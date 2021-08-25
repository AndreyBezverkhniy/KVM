#pragma once

#include <vector>
#include <string>

using namespace std;

void skipCurrentBlock(int &literalIntex);

bool parseArgumentList(int &literalIntex, vector<string> &argumentList);

bool prepareFunctions(int &failureIntex);

bool checkRecursiveImport(vector<string> &importChain,
string importingFilePath);

void popNLastLiterals(int n);

bool expandImportInstruction(vector<string> &importChain);

bool endsByImportInstruction();

void calculateNextCursorPosition(int &line, int &column, char character);

void closeFileAndPopFromImportChain(FILE *fp, vector<string> &importChain);

bool readSourceFile(vector<string> &importChain, string sourceFilePath);

bool readProgram(string sourceFilePath);
