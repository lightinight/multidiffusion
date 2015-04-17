#ifndef _DEFS_H
#include "defs.h"
#endif
#ifndef _PARAMETER_H
#include "parameter.h"
#endif
#define _MOBILITY_H
//define the pointer-type for the functions of 
//mobility and thermodynamic factor.
typedef double(*funcpointert)(std::vector<double>, double);
typedef double(*funcpointerk)(std::vector<double>, double, std::vector<double>);
class Variable
{
    //this is a class using for modifing the object of 
    //ModifiedMobility class.
public:
    int elementindex;
    int  parameter_order;
    std::vector<int> constituent;
    int seriesindex;
    Variable() {;}
    Variable(int, std::vector<int>, int, int);
    void setvariable(int, std::vector<int>, int, int);
};
class Mobility
{
    //this is a kinetic database.
protected:
    std::map<int, std::vector<Parameter> > db;
public:
    Mobility() {;}
    Mobility(const Mobility&);
    void setmobility(const Mobility&);
    void additem(int, std::vector<Parameter>);
    double evaluate(int, std::vector<double> x, double T);
    void display();
    double& getpointer(Variable);
};
class Diffusivity
{
    //a database of mobility and thermodynamic factor.
private:
	std::vector< std::vector<double> > di;
    std::vector< std::vector<double> > dc;
	std::vector< std::vector<funcpointert> > tf;
	Mobility m;
public:
	Diffusivity() {;}
    Diffusivity(const Diffusivity&);
    ~Diffusivity(){;}
	void  set_comp_num(int cnum);
	void setthermofactor(std::vector< std::vector<funcpointert> > tf_);
    void setmobility(Mobility);
	std::vector< std::vector<double> > getintrinsicmatrix(std::vector<double> x, double T, std::vector<double>);
    std::vector< std::vector<double> > getchemicalmatrix(std::vector<double> x, double T, int ref=-1);
    double& getpointer(Variable);
};
