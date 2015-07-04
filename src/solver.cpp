#include "solver.h"

using namespace std;

void solver::CalculateLaplacian(openphase::Storage3D<double, 1> &var, openphase::Storage3D<double, 1> &Laplacian)
{
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn = var.sizeN();

    //stencil for 3d fem dicretization
    /*
    double stencil[3][3][3] = {
        {
          0, 0, 0,
          0, 1.0, 0,
          0, 0, 0
        },
        {
          0, 1.0, 0,
          1.0, -6.0, 1.0,
          0, 1.0, 0
        },
        {
          0, 0, 0,
          0, 1.0, 0,
          0, 0, 0
        },
    };
    */

    double stencil[3] = {1.0, -2.0, 1.0};

    for(int nx=1; nx<Nx-1; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            double dc = 0;
            for(int tx=-1; tx<=1; ++tx)
            for(int ty=-1; ty<=-1; ++ty)
            for(int tz=-1; tz<=-1; ++tz)
            {
                dc += stencil[1+tx]*var(nx+tx, ny+ty, nz+tz, ci);
            }
            /*
            for(int cj=0; cj<Nn; ++cj)
            {
                //if(ci==cj) continue;
                for(int tx=-1; tx<=1; ++tx)
                for(int ty=-1; ty<=1; ++ty)
                for(int tz=-1; tz<=1; ++tz)
                {
                    dc += stencil[1+tx][1+ty][1+tz]*var(nx+tx, ny+ty, nz+tz, cj);
                }
            }
            */
            Laplacian(nx, ny, nz, ci) = dc;
        }
    }
}

void solver::Zeroflux_Boundary(openphase::Storage3D<double, 1> &var)
{
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn = var.sizeN();

    //y0z plane
    int xbegin, ybegin, zbegin, xend, yend, zend;
    xbegin=0, ybegin=0, zbegin=0, xend=1, yend=Ny, zend=Nz;
    for(int nx=xbegin; nx<xend; ++nx)
    for(int ny=ybegin; ny<yend; ++ny)
    for(int nz=zbegin; nz<zend; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            var(nx, ny, nz, ci) = var(xend, ny, nz, ci);
        }
    }

    //ynz plane
    xbegin=Nx-1, ybegin=0, zbegin=0, xend=Nx, yend=Ny, zend=Nz;
    for(int nx=xbegin; nx<xend; ++nx)
    for(int ny=ybegin; ny<yend; ++ny)
    for(int nz=zbegin; nz<zend; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            var(nx, ny, nz, ci) = var(xbegin-1, ny, nz, ci);
        }
    }
}

void solver::Add(openphase::Storage3D<double, 1> &var1, openphase::Storage3D<double, 1> &var2)
{
    int Nx = var1.sizeX();
    int Ny = var1.sizeY();
    int Nz = var1.sizeZ();
    int Nn = var1.sizeN();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            var1(nx, ny, nz, ci) += var2(nx, ny, nz, ci);
        }
    }
}

void solver::Add(openphase::Storage3D<double, 1> &var, double var1)
{
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn = var.sizeN();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            var(nx, ny, nz, ci) += var1;
        }
    }
}

void solver::Multiple(openphase::Storage3D<double, 1> &var1, openphase::Storage3D<double, 1> &var2)
{
    int Nx = var1.sizeX();
    int Ny = var1.sizeY();
    int Nz = var1.sizeZ();
    int Nn = var1.sizeN();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            var1(nx, ny, nz, ci) *= var2(nx, ny, nz, ci);
        }
    }
}

void solver::Multiple(openphase::Storage3D<double, 1> &var, double var1)
{
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn = var.sizeN();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            var(nx, ny, nz, ci) *= var1;
        }
    }
}


void solver::Sqrt(openphase::Storage3D<double, 1> &var)
{
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn = var.sizeN();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            var(nx, ny, nz, ci) = sqrt(var(nx, ny, nz, ci));
        }
    }
}

void solver::Limit(openphase::Storage3D<double, 1> &var)
{
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int Nn = var.sizeN();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    {
        //loop for each component
        for(int ci=0; ci<Nn; ++ci)
        {
            if(var(nx, ny, nz, ci)>1.0)
            {
                var(nx, ny, nz, ci) = 1.0;
            }
            else if(var(nx, ny, nz, ci)<0.0)
            {
                var(nx, ny, nz, ci) = 0.0;
            }
        }
    }
}

double solver::Max(openphase::Storage3D<double, 1> &var)
{
    double maxvalue = 0.0;
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int NN = var.sizeN();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    for(int nn=0; nn<NN; ++nn)
    {
        if(var(nx, ny, nz, nn) > maxvalue)
        {
            maxvalue = var(nx, ny, nz, nn);
        }
    }
    return maxvalue;
}

double solver::Max(openphase::Storage3D<double, 2> &var)
{
    double maxvalue = 0.0;
    int Nx = var.sizeX();
    int Ny = var.sizeY();
    int Nz = var.sizeZ();
    int N1 = var.sizeN1();
    int N2 = var.sizeN2();
    for(int nx=0; nx<Nx; ++nx)
    for(int ny=0; ny<Ny; ++ny)
    for(int nz=0; nz<Nz; ++nz)
    for(int n1=0; n1<N1; ++n1)
    for(int n2=0; n2<N2; ++n2)
    {
        if(var(nx, ny, nz, n1, n2) > maxvalue)
        {
            maxvalue = var(nx, ny, nz, n1, n2);
        }
    }
    return maxvalue;
}
