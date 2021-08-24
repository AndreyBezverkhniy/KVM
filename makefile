interpretator: main.cpp read_program.hpp print_program.hpp execute_program.hpp expression.hpp function.hpp memory.hpp literal.hpp utils.hpp read_program.cpp print_program.cpp execute_program.cpp expression.cpp function.cpp memory.cpp literal.cpp utils.cpp
	g++ main.cpp read_program.cpp print_program.cpp execute_program.cpp expression.cpp function.cpp memory.cpp literal.cpp utils.cpp -std=c++11 -g -o interpretator

