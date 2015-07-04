#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef ALLELE_H
#include "allele.h"
#endif
#ifndef GROUP_H
#include "group.h"
#endif
#ifndef GA_H
#define GA_H
template <typename T>
class GA:public Group
{
public:
    GA();
    GA(int, int, std::vector<int>, std::vector< std::pair<double, double> >);
    GA(const GA &);
    GA &operator=(const GA<T> &);
    void setDefault();
    //void Initialize(int, int, std::vector<int>, std::vector< std::pair<double, double> >);  //this is function implemented in class Group
    std::vector<double> calculateFitness(Group &, T &);
    Group Selection(Group, T &, int);
    Allele bestOne(Group, T &);
    double bestFitness(Group, T &);
    Allele simpleGA(int, T &);
private:
    int MaxIteration;
    int MaxConvergenceIteration;
    double MutateRate;
    std::vector<double> RecordBestFitness;
};

template <typename T>
GA<T>::GA()
{
    setDefault();
}

template <typename T>
GA<T>& GA<T>::operator=(const GA<T> &rhs)
{
    setDefault();
    GroupNum = 0;
    Members.clear();
    for(auto member=rhs.Members.begin(); member!=rhs.Members.end(); ++member)
    {
        Insert(Allele(*member));
    }
    return *this;
}

template <typename T>
GA<T>::GA(int _groupnum, int _chrmnum,
      std::vector<int> _bitlist,
      std::vector< std::pair<double, double> > _limits
):Group(_groupnum, _chrmnum, _bitlist, _limits)
{
    setDefault();
}

template <typename T>
GA<T>::GA(const GA &ga):Group(ga)
{
    setDefault();
}

template <typename T>
void GA<T>::setDefault()
{
    MaxIteration = 100000;
    MaxConvergenceIteration = 20;
    MutateRate = 0.1;
}

template <typename T>
std::vector<double> GA<T>::calculateFitness(Group &pgroup, T &obj)
{
    std::vector<double> fitness;
    int gn = pgroup.GroupNum;
    for(int ci = 0; ci<gn; ++ci)
    {
     
        std::vector<double> vars;
        std::vector<double> values = pgroup.Members[ci].getValue();
        int length = pgroup.ChrmNum;
        for(int i=0; i<length; i++)
        {
            vars.push_back(limits[i].first+(limits[i].second - limits[i].first)*values[i]);
        }
        fitness.push_back(obj.getFitness(vars));
    }
    return fitness;
}

template <typename T>
Group GA<T>::Selection(Group pgroup, T &obj, int selectedsize)
{
    std::vector<double> fitness = GA<T>::calculateFitness(pgroup, obj);
    int gn = pgroup.GroupNum;
    
    double fitmin, fitmax;
    fitmin = fitness[0];
    fitmax = fitness[0];
    for(int i=0; i<gn; i++)
    {
        if(fitmin>fitness[i])
        {
            fitmin = fitness[i];
        }
        if(fitmax<fitness[i])
        {
            fitmax = fitness[i];
        }
    }
    std::vector<double> ufitness;
    std::vector<double> afitness;
    //unified the fitness array and accumulated array
    double tmp = 0;
    for(int i=0; i<gn; i++)
    {
        double t = (fitness[i]-fitmin)/(fitmax-fitmin+1.0e-20);
        ufitness.push_back(t);
        tmp += t;
    }
    double ttmp = 0;
    for(int i=0; i<gn; i++)
    {   
        ttmp += ufitness[i];
        afitness.push_back(ttmp/tmp);
    }
    int x = 0;
    for(auto num=ufitness.begin(); num!=ufitness.end(); ++x, ++num)
    {
        //std::cout<<x<<" "<<*num<<std::endl;
    }
    //roulette
    Group sgroup;
    for(int i=0; i<selectedsize; i++)
    {
        double rannum = rand()%10000/10000.0;
        int selectednum = 0;
        if(rannum<afitness[0])
        {
            selectednum = 0;
        }
        else
        {
            for(int j=1; j<gn; j++)
            {
                if(rannum>=afitness[j-1] && rannum<=afitness[j])
                {
                    selectednum = j;
                    break;
                }
            }
        }
        sgroup.Insert(pgroup.Members[selectednum]);
    }
    return sgroup;
}

template <typename T>
Allele GA<T>::bestOne(Group pgroup, T &obj)
{
    std::vector<double> fitness = GA<T>::calculateFitness(pgroup, obj);
    int tag = 0;
    int gn = pgroup.GroupNum;
    double tmp = fitness[0];
    for(int i=0; i<gn; ++i)
    {
	    if(tmp<=fitness[i])
	    {
		    tmp = fitness[i];
		    tag = i;
	    }
    }
    RecordBestFitness.push_back(fitness[tag]);
    return Allele(pgroup.Members[tag]);

}

template <typename T>
double GA<T>::bestFitness(Group pgroup, T &obj)
{   
    std::vector<double> fitness = GA<T>::calculateFitness(pgroup, obj);
    int gn = pgroup.GroupNum;
    double tmp = fitness[0];
    int tag = 0;
    for(int i=0; i<gn; i++)
    {
	    if(tmp<fitness[i])
	    {
		    tmp = fitness[i];
		    tag = i;
	    }
    }
    std::vector<double> bestvars;
    int i = 0;
    for(auto member=pgroup.Members.begin(); member!=pgroup.Members.end(); ++i, ++member)
    {
        std::vector<double> values = member->getValue();
        std::vector<double> fvalues;
        for(int i=0; i<ChrmNum; ++i)
        {
            fvalues.push_back(limits[i].first+(limits[i].second - limits[i].first)*values[i]);
        }
        if(i==tag)
        {
            bestvars = fvalues;
        }
        for(auto i=0; i<ChrmNum; ++i)
        {
            std::cout<<"bit: "<<i<<"\t"<<bitlist[i]<<"\t"\
                 <<"\t"<< std::setprecision(7)\
                 <<values[i]<<"\t"<<fvalues[i]<<"\t"\
                 <<std::endl;
        }
    }
    std::cout<<"best fitness is at member "<<tag<<std::endl;
    for(auto var=bestvars.begin(); var!=bestvars.end(); ++var)
    {
        std::cout<<*var<<" ";
    }
    std::cout<<std::endl;
    return tmp;

}

template <typename T>
Allele GA<T>::simpleGA(int times, T &obj)
{
    RecordBestFitness.clear();

	double MutateRate = 0.01;
	double selectedrate = 0.5;
	Group Parents = Group::newGroup(GroupNum);
	//loop
	int count = 0;
	Allele Best;
	for(int tstep=0; tstep<times; ++tstep)
	{
		std::cout<<"\n\n"<<tstep<<std::endl;
		Group Selecteds = GA<T>::Selection(Parents, obj, GroupNum);
		Group OffSpring = GA<T>::crossOverGroup(Selecteds);
		Group MutatedGroup = Group::mutateGroup(OffSpring, MutateRate);
		Allele tBest = GA<T>::bestOne(Parents, obj);
		std::cout<<"best member of generation "<<tstep<<" is:"<<std::endl;
		Best.show();
		MutatedGroup += Parents;
		Parents = GA<T>::Selection(MutatedGroup, obj, selectedrate*GroupNum);
		Parents.Insert(tBest);
		int newnum = GroupNum - Parents.GroupNum;
		Group tmpGroup = Group::newGroup(newnum);
		Parents += tmpGroup;
		
		if(Best == tBest)
		{
		    count++;
		}
		else
		{
		    count = 0;
		}
		if(count == MaxConvergenceIteration)
		{
		    std::cout<<"Reach max convergence iterations. Loop terminate!"
		             <<std::endl;
		    break;
		}
		Best = tBest;
	}
	
	GA<T>::bestFitness(Parents, obj);
	return GA<T>::bestOne(Parents, obj);
}
#endif

