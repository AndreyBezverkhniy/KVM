#include "element.h"
#include "utils.h"

Element::Element(){}
bool Element::SaveInner(ostream &os) const {
	return true;
}
bool Element::LoadInner(istream &is){
	return true;
}
