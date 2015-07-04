#include "diffusion.h"

Diffusion::Diffusion(const Couple& _couple)
{
    couple = _couple;
    couple.makeInfo(info);
}

Diffusion::Diffusion(const Diffusion& diffusion)
{
    couple = diffusion.couple;
    couple.makeInfo(info);
}

Diffusion& Diffusion::operator=(const Diffusion &rhs)
{
    if(this != &rhs)
    {
        couple = rhs.couple;
        couple.makeInfo(info);
    }
    return *this;
}

void Diffusion::setCouple(const Couple &_couple)
{
    couple = _couple;
    couple.makeInfo(info);
}

void Diffusion::setDiffusivity(const Diffusivity &diff)
{
    d = diff;
}

void Diffusion::setSymTable(std::map<std::string, double> &_symtable)
{
    symtable.insert(_symtable.begin(), _symtable.end());
}

void Diffusion::setVariables(std::vector<double> &_vars)
{
    int i = 0;
    for(auto each=symtable.begin(); each!=symtable.end(); ++each, ++i)
    {
        symtable[each->first] = _vars[i];
    }
    d.setSymTable(symtable);
}

void Diffusion::setSimulation(double _dx, double _dt)
{
    dx = _dx;
    dt = _dt;
}

double Diffusion::getMaxCoeff()
{
    maxcoeff = solver::Max(model.getD());
}

double Diffusion::getError()
{
    model.initialize(info, dx);
    model.simulation(info, d, dt);
    openphase::Storage3D<double, 1> &var = model.getConc();
    openphase::Storage3D<double, 1> &mvar = couple.getMconc();
    double error = 0.0;
    
    int NX = var.sizeX();
    int NY = var.sizeY();
    int NZ = var.sizeZ();
    int NN = var.sizeN();
        
    for(int nn=0; nn<NN; ++nn)
    {
        double cmax = abs(var(0, 0, 0, nn) - mvar(0, 0, 0, nn));
        cmax = cmax*cmax;
        double cmin = abs(var(0, 0, 0, nn) - mvar(0, 0, 0, nn));
        cmin = cmin*cmin;
        
        for(int nx=0; nx<NX; ++nx)
        for(int ny=0; ny<NY; ++ny)
        for(int nz=0; nz<NZ; ++nz)
        {
            double e = abs(var(nx, ny, nz, nn) - mvar(nx, ny, nz, nn));
            e = e*e;
            if(cmax<e) cmax = e;
            if(cmin>e) cmin = e;
        }

        for(int nx=0; nx<NX; ++nx)
        for(int ny=0; ny<NY; ++ny)
        for(int nz=0; nz<NZ; ++nz)
        {
            double e = abs(var(nx, ny, nz, nn) - mvar(nx, ny, nz, nn));
            e = e*e;
            error += (e - cmin)/(cmax - cmin + 1.0e-20);
        }
    }
    return error;
}

double Diffusion::getFitness()
{
    return 1.0/getError();
}
