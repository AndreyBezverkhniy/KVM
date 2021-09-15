files = read_program print_program execute_program expression function memory literal utils operand_object inbuilt_functions

headers = $(files)
sources = $(files)

headers += errors
sources += main

headerPathes = $(addprefix ./src/,$(headers:=.hpp))
sourcePathes = $(addprefix ./src/,$(sources:=.cpp))


kvm: $(headerPathes) $(sourcePathes)
	g++ $(sourcePathes) -std=c++11 -g -o kvm

