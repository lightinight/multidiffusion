#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include "alcusi_fcc.h"

int main()
{
    vector<double> x1 = {0.7, 0.2, 0.1};  //mole-fraction
    vector<double> x2 = {0.1, 0.7, 0.2};  //mole-fraction
    vector<double> x3 = {0.1, 0.2, 0.7};  //mole-fraction
    double T = 900;  //kelvin
    cout<<"dmu(al)_d(xal): "<<TF_FCC_A1_AL_AL(x1, T)<<endl;
    cout<<"dmu(al)_d(xcu): "<<TF_FCC_A1_AL_CU(x1, T)<<endl;
    cout<<"dmu(al)_d(xsi): "<<TF_FCC_A1_AL_SI(x1, T)<<endl;
    
    cout<<"dmu(cu)_d(xal): "<<TF_FCC_A1_CU_AL(x1, T)<<endl;
    cout<<"dmu(cu)_d(xcu): "<<TF_FCC_A1_CU_CU(x1, T)<<endl;
    cout<<"dmu(cu)_d(xsi): "<<TF_FCC_A1_CU_SI(x1, T)<<endl;
    
    cout<<"dmu(si)_d(xal): "<<TF_FCC_A1_SI_AL(x1, T)<<endl;
    cout<<"dmu(si)_d(xcu): "<<TF_FCC_A1_SI_CU(x1, T)<<endl;
    cout<<"dmu(si)_d(xsi): "<<TF_FCC_A1_SI_SI(x1, T)<<endl;
}
