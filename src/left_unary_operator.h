#pragma once
#include "unary_operator.h"

#define LEFT_UNARY_PLUS string("+")
#define LEFT_UNARY_MINUS string("-")
#define LEFT_UNARY_INC string("++")
#define LEFT_UNARY_DEC string("--")
#define LEFT_UNARY_NOT string("!")

class LeftUnaryOperator : public UnaryOperator{};
