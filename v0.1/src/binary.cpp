#include "binary.h"

using namespace std;

binary::binary(int bits)
{
    binstr = randombinary(bits);
}

string binary::randombinary(int bits)
{
    
    string chrn = "";
    for(int i=0; i<bits; i++)
    {
        string tmp = to_string(rand()%2);
        chrn += tmp;
    }
    return chrn;
}

LLONG_BIN binary::decode(string _binstr)
{
    LLONG_BIN number = stol(_binstr, nullptr, 2);
    return number;
}

string binary::encode(LLONG_BIN number)
{
    LLONG_BIN tmp = number;
    string _binstr = "";
    while(tmp!=0)
    {
        string temp = to_string(tmp%2);
        tmp = (tmp-tmp%2)/2;
        _binstr = temp + _binstr;
    }
    return _binstr;
}

void binary::exchange(string *str1, string *str2, int pos)
{
    string tmp;
    string tmpstr1(*str1, 0, pos);
    int len = str1->size();
    tmp.assign(*str2, pos, len-pos);
    tmpstr1 += tmp;
    
    string tmpstr2(*str2, 0, pos);
    tmp = "";
    tmp.assign(*str1, pos, len-pos);
    tmpstr2 += tmp;
    *str1 = tmpstr1;
    *str2 = tmpstr2;
}

void binary::exchange(binary *bin1, binary *bin2, int pos)
{
    string cstr1 = bin1->binstr;
    string cstr2 = bin2->binstr;
    bin1->exchange(&cstr1, &cstr2, pos);
    bin1->set(cstr1);
    bin2->set(cstr2);
}

string binary::substitute(string str, string substr, int pos)
{
    str = str.replace(pos, substr.size(), substr.c_str());
    return str;
}

string binary::minimum(int bits)
{
    string chrn = "";
    for(int i=0; i<bits; i++)
    {
        chrn += "0";
    }
    return chrn;
}

string binary::maximum(int bits)
{
    string chrn = "";
    for(int i=0; i<bits; i++)
    {
        chrn += "1";
    }
    return chrn;
}
