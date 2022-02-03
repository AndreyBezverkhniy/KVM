#pragma once
#include <string>
#include <iostream>

using namespace std;

bool USave(ostream &os,bool b);
bool ULoad(istream &is,bool &b);
bool USave(ostream &os,char ch);
bool ULoad(istream &is,char &ch);
bool USave(ostream &os,int n);
bool ULoad(istream &is,int &n);
bool USave(ostream &os,string str);
bool ULoad(istream &is,string &str);
