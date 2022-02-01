#include "body.h"
#include "utils.h"

Body::Body(){}
bool Body::Save(ostream &os) const {
	int size=(int)instructions.size();
	if(!USave(os,size)){
		return false;
	}
	return true;
}
bool Body::Load(istream &is){
	int size;
	if(!ULoad(is,size)){
		return false;
	}
	return true;
}
