#ifndef DEFS_H
#include "defs.h"
#endif

#ifndef STORAGES_H
#include "storages.h"
#endif

#ifndef STRTOOL_H
#include "strtool.h"
#endif

#ifndef INFO_H
#define INFO_H

class Info
{
public:
    //from the object of Class Couple
    std::string Couplename;

    int Time;

    double Temperature;

    double PosLeft;
    double PosRight;
    double PosInit;
    
    std::map<std::string, double> Cleft;
    std::map<std::string, double> Cright;

    std::vector<std::string> Symbols;

    void show();
};
#endif

