#pragma once

#include "literal.hpp"

bool parseExactLiteral(int &literalIntex, Literal literal,
bool printError = true);

bool parseExactLiteral(int &literalIntex, string value,
bool printError = true);
