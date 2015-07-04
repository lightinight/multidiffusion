#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef PROFILE_H
#include "profile.h"
#endif
#ifndef COUPLE_H
#include "couple.h"
#endif
#ifndef INFO_H
#include "info.h"
#endif
#ifndef DIFFUSIVITY_H
#include "diffusivity.h"
#endif
#ifndef STORAGES_H
#include "storages.h"
#endif
#ifndef MODEL_H
#include "model.h"
#endif
#ifndef DIFFUSION_H
#define DIFFUSION_H
class Diffusion
{
public:
    /*
    Example:
        Diffusion diff;
        diff.setCouple(couple);
        diff.setDiffusivity(Diffusivity);
        
    */
    Diffusion() {;}
    Diffusion(const Couple &);
    Diffusion(const Diffusion &);
    Diffusion& operator=(const Diffusion &);
    
    void setCouple(const Couple &);
    void setDiffusivity(const Diffusivity &diff);
    void setSymTable(std::map<std::string, double> &); //called by user
    void setVariables(std::vector<double> &); //called by optimization, setSymTable should be called before this
    void setSimulation(double, double); //before optimization
    double getMaxCoeff();
    double getError();
    double getFitness();
private:
    Couple couple;
    Info info;
    Diffusivity d;
    Model model;
    
    std::map<std::string, double> symtable;
    
    double maxcoeff;
    double dx;
    double dt;
};
#endif

