#ifndef _DEFS_H
#include "defs.h"
#endif

#define _BINARY_H

typedef long LLONG_BIN;

class binary
{
private:
    std::string binstr;
public:
    binary(){};
    binary(int bits);
    binary(std::string binstr_) {binstr=binstr_;}
    std::string randombinary(int bits);
    void set(std::string str){binstr=str;}
    std::string get(){return binstr;}
    int getbits() {return binstr.size();}
    LLONG_BIN decode(std::string _binstr);
    std::string encode(LLONG_BIN);
    void exchange(std::string *str1, std::string *str2, int pos);
    void exchange(binary *str1, binary *str2, int pos);
    std::string substitute(std::string str, std::string substr, int pos);
    std::string minimum(int bits);
    std::string maximum(int bits);
};
