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
	//set the component number and grid number
	void setSystem(int, int);
	//assign a diffusivity database to this class
    void setDiffusivity(const Diffusivity&);
	//two functions needed for setSystem to initialize the variables
	void __initVariable(std::vector< std::vector<double> >* _var, int _cnum, int _gridnum);
	void __setVariable(std::vector< std::vector<double> >* _var, double _value);
	//set the initial condition for the simulation.
	void setCouple(std::vector<int>, std::vector<double>, std::vector<double>);
	//calculate the increment for the finite differential algorithm
	void calculateIncrement(double T);
	//update the variables
	void update(double, double);
	//calculate the increment and update
	void evolution(int, double, double, double);
	//output the result to files(needed for outputAll
	void outputVariable(std::vector< std::vector<double> > _var, string _pathname);
	//output All the conc to a specific path
	void outputAll(string _pathname);
	
	//for unit test
	void checkConserved();
};
