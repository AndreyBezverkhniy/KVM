#pragma once
#include "operand.h"

class Element : public Operand{
public:
	Element();
	bool SaveInner(ostream &os) const;
	bool LoadInner(istream &is);
};
