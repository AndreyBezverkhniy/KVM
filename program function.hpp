#pragma once

#include <vector>
#include <string>

using namespace std;

struct FunctionDescription {
    int bodyPointer;
    vector<string> arguments;
    FunctionDescription() = default;
    FunctionDescription(int _bodyPointer) {
        bodyPointer = _bodyPointer;
    }
};
