#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef STORAGES_H
#include "storages.h"
#endif
#ifndef DIFFUSIVITY_H
#define DIFFUSIVITY_H
typedef double(*tfpointer)(std::vector<double>, double);

class Diffusivity
{
public:
    Diffusivity();
    Diffusivity(const Diffusivity &diffusivity); 
    Diffusivity& operator=(const Diffusivity &rhs);
    
    void setSymbolStrings();
    void initSymbols();
    void setFuncTable(std::map<std::string, tfpointer>);
    void setSymTable(std::map<std::string, double>);
    void showSymTable();
    
    void calculateDiffusivity(
        openphase::Storage3D<double, 1> &,
        openphase::Storage3D<double, 2> &,
        double
    );
    
    double M_ANSATZ_A(std::vector<double> x, double T);
    double M_ANSATZ_B(std::vector<double> x, double T);
    double M_ANSATZ_C(std::vector<double> x, double T);  
    
    std::map<std::string, double> DI_ANSATZ(std::vector<double> x, double T);
    
private:
    std::vector<std::string> symbol_strings;
    std::map<std::string, tfpointer> FuncTable;
    std::map<std::string, double> SymTable;
    std::vector<std::string> Index;
};
#endif

