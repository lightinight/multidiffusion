#include "binary.h"

Binary::Binary(){};
Binary::Binary(std::string _binstr) 
{
    bitnum = _binstr.size();
    binstr=_binstr;
}
Binary::Binary(const Binary &bin)
{
    bitnum = bin.bitnum;
    binstr=bin.binstr;
}
Binary::Binary(int bit) 
{
    bitnum = bit;
    _randomBinary(bit);
}
Binary& Binary::operator=(const Binary &rhs)
{
    if(this != &rhs)
    {
        bitnum = rhs.bitnum;
        binstr=rhs.binstr;
    }
    return *this;
}
char Binary::operator()(int i)
{
    return binstr[i];
}
bool Binary::operator==(const Binary& rhs)
{
    return binstr == rhs.binstr;
}
void Binary::_randomBinary(int bit)
{
    binstr = "";
    for(int i=0; i<bit; i++)
    {
        char tmp = char(rand()%2+48);
        binstr += tmp;
    }
}
Binary Binary::randomBinary(int bit)
{
    std::string tbinstr = "";
    for(int i=0; i<bit; i++)
    {
        char tmp = char(rand()%2+48);
        tbinstr += tmp;
    }
    return Binary(tbinstr);
}
void Binary::setString(std::string newstr)
{
    binstr.clear();
    bitnum = newstr.size();
    binstr = newstr;
}
std::string Binary::getString()
{
    return binstr;
}
GALONG Binary::decode(Binary &bin)
{
    //from str to GALONG
    GALONG number = 0;
    for(int i=bin.bitnum-1; i>=0; i--)
    {
        int tmp = int(bin(i))-48;
        number += tmp*pow(2, bin.bitnum-i-1);
    }
    return number;
}
Binary Binary::encode(GALONG number)
{
/*copy initialization will be adopted in order to return a Binary object.*/
    GALONG tmp = number;
    std::string _binstr = "";
    while(tmp!=0)
    {
        int temp = tmp%2;  //temp = 0 or 1
        char ttmp = char(temp+48);  //ttmp = "0" or "1"
        tmp = (tmp-temp)/2;
        _binstr = ttmp + _binstr;
    }
    return Binary(_binstr);
}
void Binary::exchange(Binary &bin1, Binary &bin2, int pos)
{
    std::string bin1str = bin1.binstr;
    std::string bin2str = bin2.binstr;
    std::string tmp;
    std::string tmpstr1(bin1str, 0, pos);
    int len = bin1.bitnum;
    tmp.assign(bin2str, pos, len-pos);
    tmpstr1 += tmp;
    
    std::string tmpstr2(bin2str, 0, pos);
    tmp.clear();
    tmp.assign(bin1str, pos, len-pos);
    tmpstr2 += tmp;
    bin1.setString(tmpstr1);
    bin2.setString(tmpstr2);
}
std::string Binary::minimum(int num)
{
    std::string str = "";
    for(int i=0; i<num; i++)
    {
        str += "0";
    }
    return str;
}
std::string Binary::maximum(int bit)
{
    std::string str = "";
    for(int i=0; i<bit; i++)
    {
        str += "1";
    }
    return str;
}
int Binary::getBit()
{
    return bitnum;
}
GALONG Binary::getValue()
{
    GALONG number = 0;
    for(int i=bitnum-1; i>=0; i--)
    {
        int tmp = int(binstr[i])-48;
        number += tmp*pow(2, bitnum-i-1);
    }
    return number;
}
void Binary::test_check(Binary &bin, int bit, int value, std::string _binstr)
{
    if(bin.bitnum!=bit)
    {
        //std::cout<<"Failed with the constructor function assigned
                   //with a string!"<<std::endl;
        std::cout<<bin.bitnum<<" != "<<bit<<std::endl;
    }
    else
    {
        std::cout<<".";
    }
    if(bin.getValue()!=value)
    {
        //std::cout<<"Failed with the getvalue function!"<<std::endl;
        std::cout<<bin.getValue()<<" != "<<value<<std::endl;
    }
    else
    {
        std::cout<<".";
    }
    if(bin.getString()!=_binstr)
    {
        //std::cout<<"Failed with the getstring function!"<<std::endl;
        std::cout<<bin.getString()<<" != "<<_binstr<<std::endl;
    }
    else
    {
        std::cout<<".";
    }
}
void Binary::test_Binary()
{
    int _bitnum = 5;
    std::string _binstr = "00100";
    Binary tbin(_binstr);
    Binary tbinx = tbin;
    test_check(tbin, 5, 4, _binstr);
    Binary tbin1(tbin);
    test_check(tbin1, 5, 4, _binstr);
    Binary tbin2;
    tbin2.setString(_binstr);
    test_check(tbin2, 5, 4, _binstr);
    Binary tbin3(tbin1.maximum(_bitnum));
    test_check(tbin3, 5, 31, "11111");
    Binary tbin4(tbin1.minimum(_bitnum));
    test_check(tbin4, 5, 0, "00000");
    exchange(tbin3, tbin4, 2);
    test_check(tbin3, 5, 24, "11000");
    test_check(tbin4, 5, 7, "00111");
    exchange(tbin3, tbin4, 0);
    test_check(tbin4, 5, 24, "11000");
    test_check(tbin3, 5, 7, "00111");
    exchange(tbin3, tbin4, _bitnum);
    test_check(tbin4, 5, 24, "11000");
    test_check(tbin3, 5, 7, "00111");
    test_check(tbin, 5, 4, _binstr);
    test_check(tbinx, 5, 4, _binstr);
    std::cout<<std::endl;
}

