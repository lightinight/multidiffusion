#include "mobility.h"
using namespace std;
int test_mobility()
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
        Parameter(1, {2},       0, {-38385.93-128.05}),
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
    
    vector<double> xal = {6.9999998808E-01, 7.0999997854E-01,
                      7.2000002861E-01, 7.3000001907E-01,
                      7.4000000954E-01, 7.5000000000E-01};
    vector<double> calcmal = {9.2702180567E-13, 9.4139496545E-13,
                      9.5449917501E-13, 9.6622742359E-13,
                      9.7647627831E-13,9.8514664308E-13};
    vector<double> error = {0.000179399185035859, 0.000172338061274609, 
                0.000165339157058555, 0.000158566866808134, 
                0.000151640144753574, 0.000145272020491657,
                0.000138678002376139, 0.000132622845673014, 
                0.000125928908249682};
    int num = xal.size();
    for(int i=0; i<num; i++)
    {
        vector<double> x = {xal[i], (1.0-xal[i])/2.0, (1.0-xal[i])/2.0};
        double v = alcusi.evaluate(0, x, 900.0);
        //cout<<v<<"\t"<<calcmal[i]<<endl;
        if(abs((v-calcmal[i])/v) < 0.01)
        {
            cout<<".";
        }
    }
    cout<<endl;
    return 0;
}
