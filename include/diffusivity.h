#ifndef _DEFS_H
#include "defs.h"
#endif
#ifndef STORAGES_H
#include "storages.h"
#endif
#ifndef SETTING_H
#include "setting.h"
#endif
#ifndef TABLE_H
#include "table.h"
#endif
#ifndef DIFFUSIVITY_H
#define DIFFUSIVITY_H
typedef double(*InterDiffusivity)(std::vector<double>, double, std::vector<double>);

class Diffusivity
{
private:
    int Nx, Ny, Nz, NC;
    std::vector< std::vector<InterDiffusivity> > funcs;
    std::vector<double> vars;
public:
    void Initialize(Info &info);
    void setVariables(std::vector<double> _vars)
    {
        
    }
    void setFunc(Table<std::string, InterDiffusivity> &table, Info &info)
    {
        funcs.clear();
        for(int ci=0; ci<NC; ++ci)
        {
            std::vector<InterDiffusivity> temp;
            for(int cj=0; cj<NC; ++cj)
            {
                std::vector<std::string> key;
                key.push_back(info.Symbols[ci]);
                key.push_back(info.Symbols[cj]);
                std::cout<<info.Symbols[ci]<<"  "<<info.Symbols[cj]<<std::endl;
                temp.push_back(table(key));
            }
            funcs.push_back(temp);
        }
    }
    double operator()(int ci, int cj, std::vector<double> &c, double temperature)
    {
        return funcs[ci][cj](c, temperature, vars);
    }
};

inline void Diffusivity::Initialize(Info &info)
{
    Nx = info.getNx();
    Ny = info.getNy();
    Nz = info.getNz();
    NC = info.getNC();  
}
#endif

