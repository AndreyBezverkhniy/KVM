#include <iostream>

using namespace std;

class Instruction{
public:
	bool Save(ostream &os) const;
	bool Load(istream &is);
};
