#ifndef _DEFS_H
#include "defs.h"
#endif
#ifndef STORAGES_H
#include "storages.h"
#endif
#ifndef SETTING_H
#include "setting.h"
#endif
#ifndef SOLVER_H
#include "solver.h"
#endif
#ifndef DIFFUSIVITY_H
#include "diffusivity.h"
#endif
#ifndef CONCENTRATION_H
#define CONCENTRATION_H

//template <class T>
class Concentration
{
private:
    int NC;
    int Nx;
    int Ny;
    int Nz;
    bool is_create;
public:
    //Concentration() {is_create = 0;}
    openphase::Storage3D<double, 1> Conc;
    openphase::Storage3D<double, 1> Laplacian;
    openphase::Storage3D<double, 1> dConc;
    openphase::Storage3D<double, 2> D;
    void Initialize(Info &info);
    void updateD(Diffusivity &interd, Info &info);
    void CalculateLaplacian();
    void CalculateIncrement();
    void Update(double dt, double dx = 5.0);
};
#endif

