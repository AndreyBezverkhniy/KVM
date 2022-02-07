#pragma once
#include "function_signature.h"
#include "block.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Body{
public:
    shared_ptr<Block> block;
    vector<string> argument_names;
    Body();
	bool Save(ostream &os) const;
	bool Load(istream &is);
};
