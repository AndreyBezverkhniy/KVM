#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "literal.h"

using namespace std;

bool ReadFile(string path,vector<Literal> &vec);
bool ReadFile(istream &is,vector<Literal> &vec);
