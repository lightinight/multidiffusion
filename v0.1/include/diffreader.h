#ifndef _DEFS_H
#include "defs.h"
#endif
#define _DIFFREADER_H
class Profile
{
private:
	double time; //minutes
	double temperature; //kelvin
	std::vector<double> distance;  //micron meter
	std::vector<double> concentration;  //more fraction
	std::string couplename;
	std::string elementname;
	double cleft;
	double cright;
public:
	Profile(){}
	Profile(const Profile&);
	Profile(std::string couplename, std::string elementname, double time_, double temperature, std::vector<double> distance, std::vector<double> concentration, double cleft, double cright);
	Profile splitData(std::string);
	std::vector<Profile> read(std::string);
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
	std::string getCoupleName();
	std::string getElementName();
};

class Couple
{
    private:
        std::string couplename;
        std::map<std::string, Profile> couple;
        std::map<std::string, std::vector<double> > mcouple;
    public:
        Couple(){;}
        Couple(std::string);
        Couple(std::string, std::vector<Profile>);
        void insert(std::vector<Profile>);
        void show();
        void makeInitCon(double dx = 1);
};

std::map<std::string, Couple> diffreader(std::string);
