#pragma once
#include <vector>
#include "literal.h"
#include "expression.h"
#include "number.h"

using namespace std;

bool ReadExpression(const vector<Literal> &vec,int &index,shared_ptr<Expression> &expression);
