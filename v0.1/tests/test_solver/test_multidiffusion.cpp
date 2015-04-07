#include "parameter.h"
#include "mobility.h"
#include "multidiffusion.h"
#define R 8.314
using namespace std;
Mobility makemobility()
{
    vector< Parameter > al = {
        Parameter(0, {0},       0, {-18912.6691, -138.677817}),
        Parameter(0, {1},       0, {-32812.41, -138.28}),
        Parameter(0, {2},       0, {-38385.93, -128.05}),
        Parameter(0, {0, 1},    0, {17872.93}),
        Parameter(0, {0, 1},    1, {82836})
    };
    vector< Parameter > cu = {
        Parameter(1, {0},       0, {-24000, -133.52}),
        Parameter(1, {1},       0, {-32812.41, -138.28}),
        Parameter(1, {2},       0, {-38385.93, -128.05}),
        Parameter(1, {0, 1},    0, {-152178.84}),
        Parameter(1, {0, 1},    1, {212645.4})
    };
    vector< Parameter > si = {
        Parameter(2, {0},       0, {-30000, -131.572592}),
        Parameter(2, {1},       0, {-32812.41, -138.28}),
        Parameter(2, {2},       0, {-38385.93, -128.05}),
        Parameter(2, {0, 2},    0, {75042.04})
    };
    Mobility alcusi;
    alcusi.additem(0, al);
    alcusi.additem(1, cu);
    alcusi.additem(2, si);
    
    return alcusi;
}
#include "alcusi_liquid.h"
Diffusivity makediffusivity()
{
    Diffusivity d;
    d.set_comp_num(3);
    vector< vector<funcpointert> > tf = 
	{
		{TF_LIQUIDALAL, TF_LIQUIDALCU, TF_LIQUIDALSI},
		{TF_LIQUIDCUAL, TF_LIQUIDCUCU, TF_LIQUIDCUSI},
		{TF_LIQUIDSIAL, TF_LIQUIDSICU, TF_LIQUIDSISI}
	};
	d.setthermofactor(tf);
	Mobility m = makemobility();
    d.setmobility(m);
    return d;
}

void simulation(int time, double dx, double dt)
{
    int cnum = 3;
    int gridnum = 1000;
    vector<int> pos = {500, 500, 500};
    vector<double> cleft = {0.2, 0.5, 0.3};
    vector<double> cright = {0.3, 0.4, 0.3};
    
    MultiDiffusion model;
    model.setSystem(cnum, gridnum);
    model.setDiffusivity(makediffusivity());
    model.setCouple(pos, cleft, cright);
    model.evolution(time, 900.0, dx, dt);
    model.outputAll("./data/test");
}

int main(int argc,char **argv)
{
    double time = 1000; 
    double dx = 1.0e-5;
    double dt = 1.0e-5;
    if(argc == 1)
    {
        time = atoi(argv[0]);
    }
    if(argc == 2)
    {
        time = atoi(argv[0]);
        dx = atof(argv[1]);
    }
    if(argc == 3)
    {
        time = atoi(argv[0]);
        dx = atof(argv[1]);
        dt = atof(argv[2]);
    }
    simulation(time, dx, dt);
}
