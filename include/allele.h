#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef BINARY_H
#include "binary.h"
#endif
#ifndef ALLELE_H
#define ALLELE_H
class Allele
{
public:
    Allele();
    Allele(int, std::vector<int>);
    Allele(const Allele &);
    Allele& operator=(const Allele &);
    Binary operator()(int);
    bool operator==(const Allele &);
    void Insert(const Binary &);
    std::vector<double> getValue();
    void show();
    int getAlleleNum();
    void test_Allele();
private:
    int allelenum;
    std::vector< Binary > allele;
};
#endif

