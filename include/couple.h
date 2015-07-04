#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef PROFILE_H
#include "profile.h"
#endif
#ifndef STORAGES_H
#include "storages.h"
#endif
#ifndef SETTING_H
#include "info.h"
#endif
#ifndef COUPLE_H
#define COUPLE_H

class Couple
{
public:
    void presetting();
    Couple();
    Couple(std::string);
    Couple(std::string, std::vector<Profile>);
    Couple(const Couple& _couple);
    Couple& operator=(const Couple& _couple);
    void insert(std::vector<Profile>);
    
    double getInitPos();
    void makeInterpolated(double dx);
    void makeInfo(Info &info);
    std::string getCoupleName();
    void setName(std::string);
    std::vector< std::string > getElementsName();
    void show();
    openphase::Storage3D<double, 1>& getMconc();
    std::map<std::string, Couple> read(std::string);
    
protected:
    std::string couplename;
    std::vector<std::string> elementnames;
    std::map<std::string, Profile> couple;
    openphase::Storage3D<double, 1> mconc;
    double initpos;
    double posleft;
    double posright;
    
    bool is_getinitpos;
    
};
#endif

