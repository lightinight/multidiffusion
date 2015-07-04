/*
#coding=utf-8
# =============================================
#      Author   : Jing Zhong
#    Homepage   : https://github.com/zhongjingjogy
#    E-mail     : zhongjingjogy@gmail.com
#
#  Description  :
#  Revision     :
#  No license is available now.
#
# =============================================
*/
/*
This header is designed to provide a ga class, which can be used for global optimization.
*/
#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef BINARY_H
#define BINARY_H
typedef long GALONG;
//long long can ensure 62 bits, with a value up to 1e18.
//int can ensure 31 bits, with a value up to 1e9.
//long can ensure 62 bits, with a value up to 18.
class Binary
{
public:
    Binary();
    Binary(std::string);
    Binary(const Binary &);
    Binary(int);
    Binary& operator=(const Binary &rhs);
    char operator()(int);
    bool operator==(const Binary &);
    void _randomBinary(int);
    Binary randomBinary(int);
    void setString(std::string);
    std::string getString();
    GALONG decode(Binary &);
    Binary encode(GALONG);
    void exchange(Binary &, Binary &, int);
    std::string minimum(int num);
    std::string maximum(int bit);
    int getBit();
    GALONG getValue();
    void test_check(Binary &, int, int, std::string);
    void test_Binary();
private:
    int bitnum;
    std::string binstr; 
};
#endif
