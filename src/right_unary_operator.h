#pragma once
#include "unary_operator.h"

#define RIGHT_UNARY_PLUS string("+")
#define RIGHT_UNARY_MINUS string("-")
#define RIGHT_UNARY_INC string("++")
#define RIGHT_UNARY_DEC string("--")

class RightUnaryOperator : public UnaryOperator{};
