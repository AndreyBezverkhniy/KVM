#pragma once
#include "function_signature.h"
#include <vector>

using namespace std;

extern const FunctionSignature function_write;
extern const FunctionSignature function_read;

int exec_write(const vector<int> &vec);
int exec_read(const vector<int> &vec);
