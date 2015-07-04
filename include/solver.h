#ifndef _DEFS_H
#include "defs.h"
#endif

#ifndef STORAGES_H
#include "storages.h"
#endif

#ifndef SOLVER_H
#define SOLVER_H
namespace solver
{
void CalculateLaplacian(openphase::Storage3D<double, 1> &var, openphase::Storage3D<double, 1> &Laplacian);
void Zeroflux_Boundary(openphase::Storage3D<double, 1> &var);
void Add(openphase::Storage3D<double, 1> &var1, openphase::Storage3D<double, 1> &var2);
void Add(openphase::Storage3D<double, 1> &var, double var1);
void Multiple(openphase::Storage3D<double, 1> &var1, openphase::Storage3D<double, 1> &var2);
void Multiple(openphase::Storage3D<double, 1> &var, double var1);
void Sqrt(openphase::Storage3D<double, 1> &var);
void Limit(openphase::Storage3D<double, 1> &var);
double Max(openphase::Storage3D<double, 1> &var);
double Max(openphase::Storage3D<double, 2> &var);
}
#endif

