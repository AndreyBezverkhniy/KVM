#include <memory>
#include <iostream>

using namespace std;

template<class INNER,class OUTER> bool TypedLoadInner(istream &is,shared_ptr<OUTER> &ptr){
	ptr=make_shared<INNER>();
	auto pointer=((INNER*)ptr.get());
	return pointer->LoadInner(is);
}
