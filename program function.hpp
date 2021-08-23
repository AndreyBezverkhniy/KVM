#pragma once

#include <vector>
#include <string>

using namespace std;

struct FunctionDescription {
    int bodyIntex;
    vector<string> arguments;
    FunctionDescription() = default;
    FunctionDescription(int _bodyIntex) {
        bodyIntex = _bodyIntex;
    }
};
