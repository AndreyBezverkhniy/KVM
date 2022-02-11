cmake_minimum_required(VERSION 3.0.0)
project(kvm VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(kvm src/main.cpp
src/program.cpp
src/context.cpp
src/utils.cpp
src/function_signature.cpp
src/block.cpp
src/instruction.cpp
src/var_declaration.cpp
src/expression.cpp
src/operand.cpp
src/bin_operator.cpp
src/simple_expression.cpp
src/unary_operator.cpp
src/number.cpp
src/variable_name.cpp
src/function_call.cpp
src/return.cpp
src/while.cpp
src/if.cpp
src/code.cpp
src/lexeme.cpp
src/code_1st_level_instruction.cpp
src/code_instruction.cpp
src/code_expression.cpp
src/body.cpp
src/executor.cpp
src/return_exception.cpp
)

target_include_directories(kvm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
