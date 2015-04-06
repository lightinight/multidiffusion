#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;
#include "mobility.h"
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
	void initializeVariable(std::vector< std::vector<double> >* var, int cnum_, int gridnum_);
	void setVariable(std::vector< std::vector<double> >* var, double value);
	void setSystem(int, int);
	void setCouple(std::vector<int>, std::vector<double>, std::vector<double>);
	void calculateIncrement();
	void update();
	void outputVariable(std::vector< std::vector<double> > var, string pathname);
	void outputAll(string pathname);
	void evolution(int);
};