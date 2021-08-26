files = read_program print_program execute_program expression function memory literal utils operand_object inbuilt_functions

headers = $(addprefix ./src/,$(files:=.hpp))

sources = ./src/main.cpp $(addprefix ./src/,$(files:=.cpp))

kvm: $(headers) $(sources)
	g++ $(sources) -std=c++11 -g -o kvm

