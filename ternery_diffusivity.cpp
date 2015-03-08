#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "multidiffusion.h"
/*
this part to test the Mobility class.
*/
Mobility makemobility()
{	
    //specify parameters for different interaction parameters.
    //the parameters for a Parameter means: 
    //element-index, constituent,  parameter_order, polynomial-coefficient
    vector< Parameter > al = {
        Parameter(0, {0},       0, {-123111.6, -97.37}),
        //Parameter(0, {1},       0, {-112499, -81.26}),
        //Parameter(0, {2},       0, {-155634.921, -81.0257}),
        //Parameter(0, {0, 2},    0, {1846710.24}),
        //Parameter(0, {0, 1, 2}, 0, {9.97320005e3})
    };
    vector< Parameter > mg = {
        Parameter(1, {0},       0, {-121268.4, -89.95}),
        //Parameter(1, {1},       0, {-112499, -81.26}),
        //Parameter(1, {2},       0, {-155634.921, -81.0257}),
        //Parameter(1, {0, 1},    0, {84924.19}),
        //Parameter(1, {0, 1, 2}, 0, {-1.20434323E5})
    };
    vector< Parameter > si = {
        Parameter(2, {0},       0, {-117600, -93.046}),
        //Parameter(2, {1},       0, {-112499, -81.26}),
        //Parameter(2, {2},       0, {-155634.921, -81.0257}),
        //Parameter(2, {0, 2},    0, {-152613.876}),
        //Parameter(2, {0, 1, 2}, 0, {9.97320005e3})
    };
    //define a modified-mobility object
    Mobility almgsi;
    //add the kinetic-data for different elements
    almgsi.additem(0, al);
    almgsi.additem(1, mg);
    almgsi.additem(2, si);
	return almgsi;
}

//an example shows how to use the diffusivity class
#include "alcusi_fcc.h"
Diffusivity makediffusivity()
{
    //define a Diffusivity object
    Diffusivity d;
    //allocate memory-storage for a Diffusivity obejct
	d.set_comp_num(3);
    
    //define the thermodynamic factor
    vector< vector<funcpointert> > tf = 
	{
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI},
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI},
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI}
	};
    //set a thermodynamic database
	d.setthermofactor(tf);
    //set the mobility database 
    d.setmobility(makemobility());
    return d;
}

void show(vector< vector<double> > d)
{
    cout<<"Diffusivity"<<endl;
    cout<<"    "<<"D11: "<<d[0][0]<<endl;
    cout<<"    "<<"D12: "<<d[0][1]<<endl;
    cout<<"    "<<"D21: "<<d[1][0]<<endl;
    cout<<"    "<<"D22: "<<d[1][1]<<endl;
}

int main()
{
    vector<double> x1 = {0.7, 0.2, 0.1};  //mole-fraction
    vector<double> x2 = {0.1, 0.7, 0.2};  //mole-fraction
    vector<double> x3 = {0.1, 0.2, 0.7};  //mole-fraction
    double T = 900;  //kelvin
    Diffusivity d = makediffusivity();
    vector< vector<double> > D1 = d.getchemicalmatrix(x1, T);
    vector< vector<double> > D2 = d.getchemicalmatrix(x2, T);
    vector< vector<double> > D3 = d.getchemicalmatrix(x3, T);
    show(D1);
    show(D2);
    show(D2);
}
