#include "group.h"

Group::Group()
{
    GroupNum=0;
}
Group::Group(int _groupnum, int _chrmnum,
      std::vector<int> _bitlist,
      std::vector< std::pair<double, double> > _limits
)
{
    Initialize(_groupnum, _chrmnum, _bitlist, _limits);
}
Group::Group(const Group &obj)
{
    GroupNum = 0;
    Members.clear();
    for(auto member=obj.Members.begin(); member!=obj.Members.end(); ++member)
    {
        Insert(*member);
    }
}
Group& Group::operator=(const Group &rhs)
{
    GroupNum = 0;
    Members.clear();
    for(auto member=rhs.Members.begin(); member!=rhs.Members.end(); ++member)
    {
        Insert(Allele(*member));
    }
    return *this;
}
Group& Group::operator+=(const Group& rhs)
{
    for(auto member=rhs.Members.begin(); member!=rhs.Members.end(); ++member)
    {
        Insert(*member);
    }
    return *this;
}
void Group::Insert(const Allele &obj)
{
    GroupNum += 1;
    Members.push_back(Allele(obj));
}
void Group::Initialize(int _groupnum, int _chrmnum,
                std::vector<int> _bitlist,
                std::vector< std::pair<double, double> > _limits
)
{
    GroupNum = 0;
    setChrmNum(_chrmnum);
    setBitList(_bitlist);
    setLimits(_limits);
    
    Members.clear();
    for(auto i=0; i<_groupnum; ++i)
    {
        Insert(Allele(ChrmNum, bitlist));
    }
}
void Group::setChrmNum(int _num)
{
    ChrmNum = _num;
}
void Group::setBitList(std::vector<int> _bits)
{   
    bitlist.clear();
    for(auto bit=_bits.begin(); bit!=_bits.end(); ++bit)
    {
        bitlist.push_back(*bit);
    }
}
void Group::setLimits(std::vector< std::pair<double, double> > _limits)
{
    limits.clear();
    for(auto eachpair=_limits.begin(); eachpair!=_limits.end(); ++eachpair)
    {
        limits.push_back(std::make_pair(eachpair->first, eachpair->second));
    }
}
void Group::Show()
{
    int i = 0;
    for(auto member=Members.begin(); member!=Members.end(); ++i, ++member)
    {
        std::cout<<"Member: "<<i<<std::endl;
        std::cout<<"Bits of chrm: "<<ChrmNum<<std::endl;
        std::vector<double> values = member->getValue();
        std::vector<double> fvalues;
        for(int i=0; i<ChrmNum; ++i)
        {
            fvalues.push_back(limits[i].first+(limits[i].second - limits[i].first)*values[i]);
        }
        for(auto i=0; i<ChrmNum; ++i)
        {
            std::cout<<"bit: "<<i<<"\t"<<bitlist[i]<<"\t"\
                 <<"\t"<< std::setprecision(7)\
                 <<values[i]<<"\t"<<fvalues[i]<<"\t"\
                 <<std::endl;
        }
    }
}
Group Group::newGroup(int _groupnum)
{
    Group newgroup;
    for(auto i=0; i<_groupnum; ++i)
    {
        newgroup.Insert(Allele(ChrmNum, bitlist));
    }
    return newgroup;
}
Group Group::crossOverTwo(Allele &chrm1, Allele &chrm2)
{
    Allele newchrm1;
    Allele newchrm2;
    for(int i=0; i<ChrmNum; i++)
    {
        Binary bin1(chrm1(i));
        Binary bin2(chrm2(i));
        int bit = bin1.getBit();            
        int crosspos = rand()%bit;
        chrm1(i).exchange(bin1, bin2, crosspos);
        newchrm1.Insert(bin1);
        newchrm2.Insert(bin2);
        
    }
    Group tgroup;
    tgroup.Insert(newchrm1);
    tgroup.Insert(newchrm2);
    return tgroup;
}
Group Group::crossOverGroup(Group &pgroup)
{
    Group cgroup;
    int gn = pgroup.GroupNum;
    std::vector<int> index;
    for(int i=0; i<gn; ++i)
    {
        index.push_back(i);
    }
    random_shuffle(index.begin(), index.end());
    for(int i=0; i<gn/2; i++)
    {
        Group tmp = crossOverTwo(pgroup.Members[i], pgroup.Members[i+1]);
        cgroup += tmp;
    }
    return cgroup;
}
Allele Group::mutateOne(Allele chrm, int mutatemode)
{
    int chrmnum = chrm.getAlleleNum();
    Allele newchrm;
    for(int ci = 0; ci<chrmnum; ++ci)
    {
        std::string binstr = chrm(ci).getString();
        int bit = chrm(ci).getBit();
        for(int mi = 0; mi<mutatemode; ++mi)
        {
            int mutatepos = rand()%bit;
            if(int(binstr[mutatepos])-48==0)
            {
                binstr.replace(mutatepos, 1, "0");
            }
            else
            {
                binstr.replace(mutatepos, 1, "1");
            }
        }
        newchrm.Insert(Binary(binstr));
    }
    return newchrm;
}
Group Group::mutateGroup(Group &pgroup, double Mutaterate)
{
    int gn = pgroup.GroupNum;
    Group mgroup;
    for(int gi=0; gi<gn; gi++)
    {
        double mrate = rand()%1000/1000.0;
        Allele tmp;
        if(mrate<Mutaterate)
        {
            tmp = mutateOne(pgroup.Members[gi], 1);
        }
        else
        {
            tmp = pgroup.Members[gi];
        }
        mgroup.Insert(Allele(tmp));
    }
    return mgroup;
}

std::vector<double> Group::calculateFitness(Group& pgroup, double (*p_fitness)(std::vector<double> vars))
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
        fitness.push_back(p_fitness(vars));
    }
    return fitness;
}

Group Group::Selection(Group pgroup, double (*p_fitness)(std::vector<double> vars), int selectedsize)
{
    std::vector<double> fitness = Group::calculateFitness(pgroup, p_fitness);
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
Allele Group::bestOne(Group pgroup, double (*p_fitness)(std::vector<double> vars))
{
    std::vector<double> fitness = Group::calculateFitness(pgroup, p_fitness);
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
    return Allele(pgroup.Members[tag]);

}
double Group::bestFitness(Group pgroup, double (*p_fitness)(std::vector<double> vars))
{   
    std::vector<double> fitness = Group::calculateFitness(pgroup, p_fitness);
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
Allele Group::simpleGA(int times, double (*p_fitness)(std::vector<double> vars))
{
	double MutateRate = 0.01;
	double selectedrate = 0.5;
	Group Parents = Group::newGroup(GroupNum);
	//loop
	for(int tstep=0; tstep<times; ++tstep)
	{
		std::cout<<"\n\n"<<tstep<<std::endl;
		Group Selecteds = Group::Selection(Parents, p_fitness, GroupNum);
		Group OffSpring = Group::crossOverGroup(Selecteds);
		Group MutatedGroup = Group::mutateGroup(OffSpring, MutateRate);
		Allele Best = Group::bestOne(Parents, p_fitness);
		std::cout<<"best member of generation "<<tstep<<" is:"<<std::endl;
		Best.show();
		MutatedGroup += Parents;
		Parents = Group::Selection(MutatedGroup, p_fitness, selectedrate*GroupNum);
		Parents.Insert(Best);
		int newnum = GroupNum - Parents.GroupNum;
		Group tmpGroup = Group::newGroup(newnum);
		Parents += tmpGroup;
	}
	
	bestFitness(Parents, p_fitness);
	return Group::bestOne(Parents, p_fitness);
}
std::vector< int > Group::bitlist;
std::vector< std::pair<double, double> > Group::limits;
int Group::ChrmNum;
