#include "allele.h"

Allele::Allele()
{
    allelenum = 0;
};
Allele::Allele(int _num, std::vector<int> _bitnumlist)
{
    allelenum = 0;
    for(auto each=_bitnumlist.begin(); each!=_bitnumlist.end(); ++each)
    {
        Insert(Binary(*each));
    }
}
Allele::Allele(const Allele &obj)
{
    allelenum = 0;
    allele.clear();
    for(auto each=obj.allele.begin(); each!=obj.allele.end(); ++each)
    {
        Insert(*each);
    }
}
Allele& Allele::operator=(const Allele &rhs)
{
    if(this != &rhs)
    {
        allele.clear();
        allelenum = 0;
        for(auto each=rhs.allele.begin(); each!=rhs.allele.end(); ++each)
        {
            this->Insert(*each);
        }
    }
    return *this;
}
Binary Allele::operator()(int i)
{
    if(i<0 || i>allelenum)
    {
        //std::cout<<"() operator exceed the range of index!"<<std::endl;
        exit(0);
    }
    return Binary(allele[i]);
}
bool Allele::operator==(const Allele& obj)
{
    int tag = 0;
    if(allelenum == obj.allelenum)
    {
        tag = 1;
    }
    else
    {
        return 0;
    }
    int i = 0;
    for(auto each=allele.begin(); each!=allele.end(); ++i, ++each)
    {
        if(*each==obj.allele[i])
        {
            tag++;
        }
    }
    return tag==1+allelenum;
}
void Allele::Insert(const Binary &bin)
{
    allelenum += 1;
    allele.push_back(Binary(bin));
}
std::vector<double> Allele::getValue()
{
    std::vector< double > result;
    for(auto iter=allele.begin(); iter<allele.end(); iter++)
    {
        result.push_back(double(iter->getValue())/(pow(2.0, double(iter->getBit()))-1.0));
    }
    return result;
}
void Allele::show()
{
    std::cout<<"Bit number of allele: "<<allelenum<<std::endl;
    std::vector<double> value = getValue();
    for(auto i=0; i<allelenum; ++i)
    {
        std::cout<<"bit: "<<i<<"\t"<<allele[i].getBit()<<"\t"\
                 <<allele[i].getValue()<<"\t"<< std::setprecision(7)\
                 <<value[i]<<"\t"\
                 <<allele[i].getString()<<std::endl;
    }
}
int Allele::getAlleleNum()
{
    return allelenum;
}
void Allele::test_Allele()
{
    int _allelenum = 3;
    std::vector<int> _bitlist = {3, 4, 5};
    Allele tallele1;
    Binary bin1("010");
    Binary bin2("0110");
    Binary bin3("10001");
    tallele1.Insert(bin1);
    tallele1.Insert(bin2);
    tallele1.Insert(bin3);
    if(tallele1.allelenum!=_allelenum)
    {
        std::cout<<"F";
    }
    else
    {
        std::cout<<".";
    }
    Allele tallele2(tallele1);
    if(!(tallele1==tallele2))
    {
        std::cout<<"F";
    }
    else
    {
        std::cout<<".";
    }
    std::vector<double> values = tallele1.getValue();
    std::vector<double> objvalues = {2.0/7.0, 6.0/15.0, 9.0/31.0};
    int i = 0;
    for(auto eachv=values.begin(); eachv!=values.end(); ++eachv, ++i)
    {
        if(abs(*eachv-objvalues[i])<1.0e-20)
        {
            std::cout<<".";
        }
        else
        {
            std::cout<<"F";
        }
    }
    std::cout<<std::endl;
}

