#pragma once

#include <string>

const string errorIncorrectArgument = "Incorrect argument name";
const string errorRepeatedArgument = "Repeated name of function argument";
const string errorUserDefinedName = "Not permitted user defined name";
const string errorFunctionName = "Incorrect function name";
const string errorRepeatedFunction = "Repeated delaration of function";
const string errorFunctionDeclarationPattern =
	"Incorrect function declaration pattern. Expected";
const string errorRecursiveImport = "Recursive import of";
const string errorReadFile = "Can't read source file";

// TO PROVE: error*Pattern -> errorPattern*
const string errorInstructionPattern =
	"Incorrect instruction pattern. Expected";
const string errorVarPattern =
	"Incorrect var instruction pattern. Expected";
const string errorIfPattern =
	"Incorrect if instruction pattern. Expected";
const string errorBlockPattern =
	"Incorrect block pattern. Expected";
const string errorReturnPattern =
	"Incorrect return pattern. Expected";
const string errorFunctionCallPattern =
	"Incorrect function call pattern. Expected";
const string errorRepeatedVariableDeclaration =
	"Repeated declaration of variable in this scope";
const string errorFunctionOverloadByVariable =
	"Variable copies name of existing function";
const string errorFunctionParameters =
	"Incorrect amount of arguments passed to function";
const string errorUnknownIdentificator = "Identificator was not declared";
const string errorVariableNotExist = "Variable does not exist";
const string errorUnknownLeftUnaryOperator =
	"Left unary operator noes not exist";
const string errorUnknownRightUnaryOperator =
	"Right unary operator noes not exist";
const string errorBracketExpressionPattern =
	"Incorrect bracket expression pattern. Expected";
const string errorInvalidOperator = "Ivalid operator: ";
