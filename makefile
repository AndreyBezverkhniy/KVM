files = main literal

sourcePathes = $(addprefix ./src/,$(files:=.cpp))


kvm: $(sourcePathes)
	g++ $(sourcePathes) -std=c++11 -g -o kvm

