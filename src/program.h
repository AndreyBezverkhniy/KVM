#include <string>
#include "context.h"

using namespace std;

class Program{
public:
	Context global;
	Program();
	bool Save(string path);
	bool Save(ostream &os);
	bool Load(string path);
	bool Load(istream &is);
};
