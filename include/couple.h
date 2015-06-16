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
#include "setting.h"
#endif
#ifndef COUPLE_H
#define COUPLE_H

class Couple
{
    protected:
        std::string couplename;
        std::map<std::string, Profile> couple;
        std::vector<std::string> elementnames;
        double initpos;
        double posleft;
        double posright;
        double dx;
        bool is_interpolated;
        bool is_getinitpos;
    public:
        openphase::Storage3D<double, 1> mcouple;
        void presetting();
        Couple();
        Couple(std::string);
        Couple(std::string, std::vector<Profile>);
        void setName(std::string);
        std::vector< std::string > getElementsName();
        void insert(std::vector<Profile>);
        void show();
        double getInitPos();
        void makeConserved(std::string ElementName="NONE");
        void makeInterpolated(double dx=5.0);
        void makeInfo(Info &info);
        std::string getCoupleName();
        std::map<std::string, Couple> read(std::string);
};
#endif

