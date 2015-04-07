#ifndef _DEFS_H
#include "defs.h"
#endif
#ifndef _MOBILITY_H
#include "mobility.h"
#endif
#define _MULTIDIFFUSION_H
class MultiDiffusion
{
private:
	std::vector< std::vector<double> > conc;
	std::vector< std::vector<double> > dconc;
    Diffusivity d;
public:
	int cnum;
	int gridnum;
	MultiDiffusion(){};
    void setDiffusivity(const Diffusivity&);
	void initializeVariable(std::vector< std::vector<double> >* _var, int _cnum, int _gridnum);
	void setVariable(std::vector< std::vector<double> >* _var, double _value);
	void setSystem(int, int);
	void setCouple(std::vector<int>, std::vector<double>, std::vector<double>);
	void calculateIncrement(double T);
	void update(double, double);
	void outputVariable(std::vector< std::vector<double> > _var, string _pathname);
	void outputAll(string _pathname);
	void evolution(int, double, double, double);
};
