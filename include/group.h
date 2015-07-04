#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef BINARY_H
#include "binary.h"
#endif
#ifndef ALLELE_H
#include "allele.h"
#endif
#ifndef GROUP_H
#define GROUP_H
class Group
{
public:
    Group();
    Group(int, int, std::vector<int>, std::vector< std::pair<double, double> >);
    Group(const Group &);
    Group &operator=(const Group &);
    Group& operator+=(const Group& rhs);
    void Insert(const Allele &);
    void Initialize(int, int, std::vector<int>, std::vector< std::pair<double, double> >);
    static void setChrmNum(int);
    static void setBitList(std::vector<int>);
    static void setLimits(std::vector< std::pair<double, double> >);
    void Show();
    Group newGroup(int);
    Group crossOverTwo(Allele &, Allele &);
    Group crossOverGroup(Group &pgroup);
    Allele mutateOne(Allele, int);
    Group mutateGroup(Group &, double);
    std::vector<double> calculateFitness(Group &, double (*p_fitness)(std::vector<double>));
    Group Selection(Group , double (*p_fitness)(std::vector<double>), int);
    Allele bestOne(Group, double (*p_fitness)(std::vector<double>));
    double bestFitness(Group, double (*p_fitness)(std::vector<double>));
    Allele simpleGA(int, double (*p_fitness)(std::vector<double>));
    
public:
    int GroupNum;
    std::vector<Allele> Members;
    
    static int ChrmNum;
    static std::vector<int> bitlist;
    static std::vector< std::pair<double, double> > limits;
};
#endif

