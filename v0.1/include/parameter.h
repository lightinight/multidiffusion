#ifndef _DEFS_H
#include "defs.h"
#endif
#define _PARAMETER_H
class Polynomial
{
private:
    std::vector<double> series;
    //const + const*T + const*T*ln(T) + const/T + const*T**2
    //v1 v2 v3 v4
public:
    Polynomial();
    Polynomial(std::vector<double>);
    void setseries(std::vector<double>);
    double evaluate(double);
    double& getpointer(int);
    void display();
    void test();
};
class Parameter
{
public:
    Polynomial poly;
    int parameter_order;
    int index;
    std::string symbol;
    std::vector<int> constituent;
public:
    Parameter() {;}
    Parameter(int, std::vector<int>, int, std::vector<double>);
    void setparameter(int, std::vector<int>, int, std::vector<double>);
    double evaluate(std::vector<double> x, double var);
    void display();
};
