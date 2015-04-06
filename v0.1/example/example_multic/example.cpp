#include "multidiffusion.h"

int main()
{
    MultiDiffusion model;
    model.setSystem(3, 200);
    vector<int> pos = {100, 100, 100};
    vector<double> vleft = {0.4, 0.2, 0.4};
    vector<double> vright = {0.2, 0.3, 0.7};
    model.setCouple(pos, vleft, vright);
    model.evolution(10);
    model.outputAll("./example");
    return 0;
}
