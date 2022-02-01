#include <iostream>

using namespace std;

class Body{
public:
	Body();
	bool Save(ostream &os) const;
	bool Load(istream &is);
};
