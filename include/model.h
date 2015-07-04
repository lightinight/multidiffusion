#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef STORAGES_H
#include "storages.h"
#endif
#ifndef SETTING_H
#include "info.h"
#endif
#ifndef DIFFUSIVITY_H
#include "diffusivity.h"
#endif
#ifndef SOLVER_H
#include "solver.h"
#endif
#ifndef IO_H
#include "io.h"
#endif
#ifndef Model_H
#define Model_H
class Model
{
public:
    Model();
    Model(Info &, double);
    Model(Model &);
    Model& operator=(Model &);
    void initialize(Info &, double);
    void updateD(Diffusivity &, Info &);
    void calculateLaplacian();
    void calculateIncrement();
    void update(double);
    void simulation(Info &, Diffusivity &, double);
    openphase::Storage3D<double, 1>& getConc();
    openphase::Storage3D<double, 2>& getD();
    void output(std::string);
private:
    int NC;
    int Nx;
    int Ny;
    int Nz;
    int dx;
    
    openphase::Storage3D<double, 1> Conc;
    openphase::Storage3D<double, 1> Laplacian;
    openphase::Storage3D<double, 1> dConc;
    openphase::Storage3D<double, 2> D;
    
    bool is_create;
};
#endif

