#include "inbuilt_functions.h"

extern const FunctionSignature function_write=FunctionSignature("write",1);
extern const FunctionSignature function_read=FunctionSignature("read",0);

int exec_write(const vector<int> &vec){
    cout<<"OUT> "<<vec[0]<<endl;
    return 0;
}
int exec_read(const vector<int> &vec){
    cout<<"IN> ";
    int number;
    cin>>number;
    return number;
}
