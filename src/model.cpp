#include "model.h"

Model::Model()
{
    is_create= false;
}

Model::Model(Info &info, double _dx)
{
    Ny = 1;
    Nz = 1;
    Nx = int((info.PosRight - info.PosLeft)/dx) + 1;
    NC = info.Symbols.size();
    dx = _dx;
    
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
            if(dx*nx<=info.PosInit-info.PosLeft)
            {
                Conc(nx, ny, nz, ci) = info.Cleft[info.Symbols[ci]];
            }
            else
            {
                Conc(nx, ny, nz, ci) = info.Cright[info.Symbols[ci]];
            }
        }
    }
    
    is_create = true;;
}

Model::Model(Model &obj)
{
    Ny = 1;
    Nz = 1;
    Nx = obj.Nx;
    NC = obj.NC;
    dx = obj.dx;
    
    Conc = obj.Conc;
    Laplacian = obj.Laplacian;
    dConc = obj.dConc;
    D = obj.D;
    is_create = true;
}

Model& Model::operator=(Model &rhs)
{
    if(this != &rhs)
    {
        Ny = 1;
        Nz = 1;
        Nx = rhs.Nx;
        NC = rhs.NC;
        dx = rhs.dx;
        
        Conc = rhs.Conc;
        Laplacian = rhs.Laplacian;
        dConc = rhs.dConc;
        D = rhs.D;
        is_create = true;
    }
    return *this;
}

void Model::initialize(Info &info, double _dx)
{
    Ny = 1;
    Nz = 1;
    Nx = (info.PosRight - info.PosLeft)/_dx;
    NC = info.Symbols.size();
    dx = _dx;
    
    Conc.Reallocate(Nx, Ny, Nz, NC);
    Laplacian.Reallocate(Nx, Ny, Nz, NC);
    dConc.Reallocate(Nx, Ny, Nz, NC);
    D.Reallocate(Nx, Ny, Nz, NC, NC);
    
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        for(int ci=0; ci<NC; ++ci)
        {
            if(dx*nx<=info.PosInit-info.PosLeft)
            {
                Conc(nx, ny, nz, ci) = info.Cleft[info.Symbols[ci]];
            }
            else
            {
                Conc(nx, ny, nz, ci) = info.Cright[info.Symbols[ci]];
            }
        }
    }
    
    is_create = true;
}

void Model::updateD(Diffusivity &diff, Info &info)
{
    diff.calculateDiffusivity(Conc, D, info.Temperature);
}

void Model::calculateLaplacian()
{
    solver::CalculateLaplacian(Conc, Laplacian);
}

void Model::calculateIncrement()
{
    if(!is_create)
    {
        std::cout<<"The calculation is carrying out before \
               initializing the variables!"<<std::endl;
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

void Model::update(double dt)
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

void Model::simulation(Info &info, Diffusivity &diff, double dt)
{
    double t = info.Time;
    double tstep = 0.0;
    while(tstep<=t)
    {
        updateD(diff, info);
        calculateLaplacian();
        calculateIncrement();
        update(dt);
        tstep += dt;
        if(int(tstep/dt)%100 == 0)
        {
            std::cout<<tstep<<" s"<<std::endl;
        }
    }
}

openphase::Storage3D<double, 1>& Model::getConc()
{
    return Conc;
}

openphase::Storage3D<double, 2>& Model::getD()
{
    return D;
}

void Model::output(std::string fname)
{
    IO<double> io;
    io.writeVTK(fname, Conc);
}

