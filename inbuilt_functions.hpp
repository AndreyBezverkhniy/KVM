#pragma once

#include <vector>
#include <string>
#include "operand_object.hpp"

bool doesInbuiltFunctionExist(string functionName);

bool callInbuiltFunction(string functionName, const vector<int> &arguments,
OperandObject &result);
