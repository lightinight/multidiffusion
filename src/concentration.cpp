#include "concentration.h"
using namespace std;

//template <class T>
void Concentration::Initialize(Info &info)
{
    Nx = info.getNx();
    Ny = info.getNy();
    Nz = info.getNz();
    NC = info.getNC();

    Conc.Allocate(Nx, Ny, Nz, NC);
    Laplacian.Allocate(Nx, Ny, Nz, NC);
    dConc.Allocate(Nx, Ny, Nz, NC);
    D.Allocate(Nx, Ny, Nz, NC, NC);
    
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        for(int ci=0; ci<NC; ++ci)
        {
            if(nx<=info.Position[info.Symbols[ci]])
            {
                Conc(nx, ny, nz, ci) = info.Cleft[info.Symbols[ci]];
            }
            else
            {
                Conc(nx, ny, nz, ci) = info.Cright[info.Symbols[ci]];
            }
        }
    }
    
    is_create = 1;
}

void Concentration::updateD(Diffusivity &interd, Info &info)
{   
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        openphase::Tensor<double, 1> tc = Conc(nx, ny, nz);
        std::vector<double> c;
        double sumc = 0.0;
        for(int each=0; each<NC; ++each)
        {
            double LIMIT =  1.0e-20;
            
            if(tc(each) < LIMIT)
            {
                c.push_back(LIMIT);
            }
            else if(tc(each) > 1.0 - LIMIT)
            {
                c.push_back(1.0 - LIMIT);
            }
            else
            {
                c.push_back(tc(each));
            }
            sumc += tc(each);
        }
        //c.push_back(1.0-sumc);
        
        for(int ci=0; ci<NC; ++ci)
        for(int cj=0; cj<NC; ++cj)
        {
            D(nx, ny, nz, ci, cj) = interd(ci, cj, c, info.Temperature);
        }
    }
}

//template <class T>
void Concentration::CalculateLaplacian()
{
    solver::CalculateLaplacian(Conc, Laplacian);
}

//template <class T>
void Concentration::CalculateIncrement()
{
    if(!is_create)
    {
        cout<<"The calculation is carrying out before \
               initializing the variables!"<<endl;
    }
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        for(int ci=0; ci<NC; ++ci)
        {
            double dc = 0.0;
            for(int cj=0; cj<NC; ++cj)
            {
                dc += Laplacian(nx, ny, nz, cj)*D(nx, ny, nz, ci, cj);
            }
            dConc(nx, ny, nz, ci) = dc;
        }
    }
}

//template <class T>
void Concentration::Update(double dt, double dx)
{
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        for(int ci=0; ci<NC; ++ci)
        {
            Conc(nx, ny, nz, ci) += \
                dt/dx/dx*dConc(nx, ny, nz, ci);
        }
    }
}
