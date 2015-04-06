#include "parameter.h"
#include "mobility.h"
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
//int test_polynomial();
//int test_mobility();
int main()
{
    //test_polynomial();
    //test_mobility();
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
    vector<double> x;
    x.resize(3);
    for(int i=1; i<9; i++)
    {
        x[0] = double(i)*0.1; x[1] = (1.0-x[0])/2.0; x[2] = (1.0-x[0])/2.0;
        vector< vector<double> > v = d.getintrinsicmatrix(x, 900.0, x);
        double dicucu = DI_LIQUIDCUCU(x, 900.0);
        cout<<m.evaluate(0, x, 900.0)<<"\t"<<M_LIQUIDAL(x, 900.0)<<endl;
        cout<<m.evaluate(1, x, 900.0)<<"\t"<<M_LIQUIDCU(x, 900.0)<<endl;
        cout<<m.evaluate(2, x, 900.0)<<"\t"<<M_LIQUIDSI(x, 900.0)<<endl;
        cout<<dicucu<<"\tdi\t"<<v[1][1]<<endl;
    }
    //cout<<"pass tf m"<<endl;
    
    vector<double> xal = {6.9999998808E-01, 7.0999997854E-01,
                      7.2000002861E-01, 7.3000001907E-01,
                      7.4000000954E-01, 7.5000000000E-01};
    vector<double> calcdccucual = {4.4898929019E-09, 4.5555159645E-09,
                              4.6180397284E-09, 4.6772572482E-09,
                              4.7329340447E-09, 4.7848103257E-09};
    vector<double> error = {0.000179399185035859, 0.000172338061274609, 
                0.000165339157058555, 0.000158566866808134, 
                0.000151640144753574, 0.000145272020491657,
                0.000138678002376139, 0.000132622845673014, 
                0.000125928908249682};
    int num = 6;
    //cout<<"pass tc data"<<endl;
    for(int i=0; i<num; i++)
    {
        x[2] = (1.0-xal[i])/2.0; x[0] = xal[i]; x[1] = (1.0-xal[i])/2.0;
        //cout<<"pass x value"<<endl;
        vector< vector<double> > v = d.getchemicalmatrix(x, 900.0, 0);
        //cout<<"pass get dc"<<endl;
        //cout<<v[0][0]<<"\t"<<DC_LIQUIDCUCUAL(x, 900.0)<<endl;
        if(abs((v[0][0]-calcdccucual[i])/v[0][0]) < 0.01)
        {
            cout<<".";
        }
    }
    //test_polynomial();
    //test_mobility();
    return 1;
}
