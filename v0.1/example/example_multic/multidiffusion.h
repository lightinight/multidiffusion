#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;
class MultiDiffusion
{
private:
	std::vector< std::vector<double> > conc;
	std::vector< std::vector<double> > dconc;
public:
	int cnum;
	int gridnum;
	MultiDiffusion(){};
	void initializeVariable(std::vector< std::vector<double> >* var, int cnum_, int gridnum_);
	void setVariable(std::vector< std::vector<double> >* var, double value);
	void setSystem(int, int);
	void setCouple(std::vector<int>, std::vector<double>, std::vector<double>);
	
	void calculateIncrement();
	void update();
	void evolution(int);
	
	void outputVariable(std::vector< std::vector<double> > var, string pathname);
	void outputAll(string pathname);
};
