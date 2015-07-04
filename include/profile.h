#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef STRTOOL_H
#include "strtool.h"
#endif
#ifndef SPLINES_H
#include "splines.h"
#endif
#ifndef PROFILE_H
#define PROFILE_H
class Profile
{
public:
    Profile(){;}
    Profile(const Profile&);
    Profile& operator=(const Profile &);
    Profile(std::string couplename, std::string elementname, double time_, double temperature, std::vector<double> distance, std::vector<double> concentration, double cleft, double cright);
    
    Profile splitData(std::string);
    std::vector<Profile> read(std::string);
    std::vector<Profile> readString(std::string);
    void sort();
    Profile smooth(int times);
    void show();
    //int getPointsNum();
    double getInitPos();
    double getMinPos();
    double getMaxPos();
    double getSumS();
    double getCleft();
    double getCright();
    double getTime();
    double getTemperature();
    std::string getCoupleName();
    std::string getElementName();
    std::vector<double> getDistance();
    std::vector<double> getConcentration();
    
private:
    std::vector<double> distance;  //micron meter
    std::vector<double> concentration;  //more fraction
    double time; //minutes
    double temperature; //kelvin
    std::string couplename;
    std::string elementname;
    double cleft;
    double cright;
};
#endif

