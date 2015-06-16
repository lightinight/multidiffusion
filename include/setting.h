#ifndef DEFS_H
#include "defs.h"
#endif

#ifndef STORAGES_H
#include "storages.h"
#endif

#ifndef STRTOOL_H
#include "strtool.h"
#endif

#ifndef IO_H
#include "io.h"
#endif

#ifndef SETTING_H
#define SETTING_H

class Info
{
public:
    //from the object of Class Couple
    std::string Couplename;

    int Nx;
    int Ny;
    int Nz;

    int Time;

    double Temperature;

    std::map<std::string, int> Position;
    //position for matano plane
    std::map<std::string, double> Cleft;
    std::map<std::string, double> Cright;

    //from seeting file
    int CNum;
    //Index will be used to redirect to the name of the element
    std::vector<std::string> Symbols;
    
    int getNx()
    {
        return Nx;
    }
    
    int getNy()
    {
        return Ny;
    }
    
    int getNz()
    {
        return Nz;
    }
    
    int getNC()
    {
        return Position.size();
    }
    
    int getCNum()
    {
        return CNum;
    }
};

class Setting
{
public:
    Setting(){;}

    std::vector<std::string> CoupleNames;

    //std::map<std::string, funcpointer> FunctionTable;                        //for diffusivity

    void readInput(std::string InputFileName);
    void show();
};

#endif
//template of the input file for setting
/*
#the command used by this file is not case sensitive, while all character will
be converted into UPPER case.
COUPLE(COUPLENAME=ONE)
{
    #any line begins with '#' will be taken as annotation
    #define the symbols for each element
    #the numbers of elements defined here should be be the same as the input
    #file
	ONE=ALUMINIUM #ONE TWO THREE, REF etc. are use to label the elements
	TWO=SILICON
	REF=COOPER

    #for interdiffusivity
    ONE&ONE=ID11
    ONE&TWO=ID12
    ONE&REF=ID13
    TWO&ONE=ID21
    TWO&TWO=ID22
    TWO&REF=ID23
    REF&ONE=ID31
    REF&TWO=ID32
    REF&REF=ID33
}
*/

