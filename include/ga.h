/*
#coding=utf-8
# =============================================
#      Author   : Jing Zhong
#    Homepage   : https://github.com/zhongjingjogy
#    E-mail     : zhongjingjogy@gmail.com
#
#  Description  :
#  Revision     :
#  No license is available now.
#
# =============================================
*/
/*
This header is designed to provide a ga class, which can be used for global optimization.
*/
#ifndef _DEFS_H
#include "defs.h"
#endif
#ifndef GA_H
#define GA_H
typedef long GALONG;
//long long can ensure 62 bits, with a value up to 1e18.
//int can ensure 31 bits, with a value up to 1e9.
//long can ensure 62 bits, with a value up to 18.
class Binary
{
private:
    int bitnum;
    std::string binstr;
public:
    Binary(){};
    Binary(std::string binstr_) 
    {
        bitnum = binstr_.size();
        binstr=binstr_;
    }
    Binary(const Binary &bin)
    {
        bitnum = bin.bitnum;
        binstr=bin.binstr;
    }
    Binary(int bit) 
    {
        bitnum = bit;
        _randombinary(bit);
    }
    int getbit()
    {
        return bitnum;
    }
    char operator()(int i)
    {
        return binstr[i];
    }
    bool operator==(const Binary& bin)
    {
        return binstr == bin.binstr;
    }
    void _randombinary(int bit)
    {
        binstr = "";
        for(int i=0; i<bit; i++)
        {
            char tmp = char(rand()%2+48);
            binstr += tmp;
        }
    }
    Binary randombinary(int bit)
    {
        std::string tbinstr = "";
        for(int i=0; i<bit; i++)
        {
            char tmp = char(rand()%2+48);
            tbinstr += tmp;
        }
        return Binary(tbinstr);
    }
    void setstring(std::string str)
    {
        binstr.clear();
        bitnum = str.size();
        binstr=str;
    }
    std::string getstring()
    {
        return binstr;
    }
    GALONG decode(Binary &bin)
    {
        //from str to GALONG
        GALONG number = 0;
        for(int i=bin.getbit()-1; i>=0; i--)
        {
            int tmp = int(bin(i))-48;
            number += tmp*pow(2, bin.getbit()-i-1);
        }
        return number;
    }
    Binary encode(GALONG number)
    {
        GALONG tmp = number;
        std::string _binstr = "";
           while(tmp!=0)
           {
               int temp = tmp%2;
               char ttmp = char(temp+48);
               tmp = (tmp-temp)/2;
               _binstr = ttmp + _binstr;
           }
        return Binary(_binstr);
    }
    void exchange(Binary &bin1, Binary &bin2, int pos)
    {
        std::string bin1str = bin1.binstr;
        std::string bin2str = bin2.binstr;
        std::string tmp;
        std::string tmpstr1(bin1str, 0, pos);
        int len = bin1.getbit();
        tmp.assign(bin2str, pos, len-pos);
        tmpstr1 += tmp;
        
        std::string tmpstr2(bin2str, 0, pos);
        tmp.clear();
        tmp.assign(bin1str, pos, len-pos);
        tmpstr2 += tmp;
        bin1.setstring(tmpstr1);
        bin2.setstring(tmpstr2);
    }
    std::string minimum(int bit)
    {
        std::string str = "";
        for(int i=0; i<bit; i++)
        {
            str += "0";
        }
        return str;
    }
    std::string maximum(int bit)
    {
        std::string str = "";
        for(int i=0; i<bit; i++)
        {
            str += "1";
        }
        return str;
    }
    GALONG getvalue()
    {
        GALONG number = 0;
        for(int i=binstr.size()-1; i>=0; i--)
        {
            int tmp = int(binstr[i])-48;
            number += tmp*pow(2, binstr.size()-i-1);
        }
        return number;
    }
    void test_check(Binary &bin, int bit, int value, std::string _binstr)
    {
        if(bin.bitnum!=bit)
        {
            //std::cout<<"Failed with the constructor function assigned \
                       with a string!"<<std::endl;
            //std::cout<<bin.bitnum<<" != "<<bit<<std::endl;
        }
        else
        {
            //std::cout<<".";
        }
        if(bin.getvalue()!=value)
        {
            //std::cout<<"Failed with the getvalue function!"<<std::endl;
            //std::cout<<bin.getvalue()<<" != "<<value<<std::endl;
        }
        else
        {
            //std::cout<<".";
        }
        if(bin.getstring()!=_binstr)
        {
            //std::cout<<"Failed with the getstring function!"<<std::endl;
            //std::cout<<bin.getstring()<<" != "<<_binstr<<std::endl;
        }
        else
        {
            //std::cout<<".";
        }
    }
    void test_Binary()
    {
        int _bitnum = 5;
        std::string _binstr = "00100";
        Binary tbin(_binstr);
        Binary tbinx = tbin;
        test_check(tbin, 5, 4, _binstr);
        Binary tbin1(tbin);
        test_check(tbin1, 5, 4, _binstr);
        Binary tbin2;
        tbin2.setstring(_binstr);
        test_check(tbin2, 5, 4, _binstr);
        Binary tbin3(tbin1.maximum(_bitnum));
        test_check(tbin3, 5, 31, "11111");
        Binary tbin4(tbin1.minimum(_bitnum));
        test_check(tbin4, 5, 0, "00000");
        exchange(tbin3, tbin4, 2);
        test_check(tbin3, 5, 24, "11000");
        test_check(tbin4, 5, 7, "00111");
        exchange(tbin3, tbin4, 0);
        test_check(tbin4, 5, 24, "11000");
        test_check(tbin3, 5, 7, "00111");
        exchange(tbin3, tbin4, _bitnum);
        test_check(tbin4, 5, 24, "11000");
        test_check(tbin3, 5, 7, "00111");
        test_check(tbin, 5, 4, _binstr);
        test_check(tbinx, 5, 4, _binstr);
        //std::cout<<std::endl;
    }
};
class Chrom
{
private:
    int chrmnum;
    std::vector< Binary > chrm;
public:
    //if no bitlist is given in the constructor function, the randomConstructChrm should be called instead.
    //the copy constructor function should be called, when creating new Chrom object.
    Chrom()
    {
        chrmnum=0;
    };
    Chrom(int _chrmnum, std::vector<int> _bitlist)
    {
        chrmnum = 0;
        for(auto bit=_bitlist.begin(); bit!=_bitlist.end(); ++bit)
        {
            Insert(Binary(*bit));
        }
    }
    Chrom(const Chrom& _chrom)
    {
        chrmnum = 0;
        for(auto i=0; i<_chrom.chrmnum; ++i)
        {
            Insert(Binary(_chrom.chrm[i]));
        }
    }
    void Insert(Binary bin)
    {
        chrmnum += 1;
        chrm.push_back(Binary(bin));
    }
    int getChrmNum()
    {
        return chrmnum;
    }
    std::vector<double> getValue()
    {
        std::vector< double > result;
        int i=0;
        for(auto iter=chrm.begin(); iter<chrm.end(); i++, iter++)
        {
            result.push_back(double(iter->getvalue())/(pow(2.0, double(iter->getbit()))-1.0));
        }
        return result;
    }
    Binary operator()(int i)
    {
        if(i<0 || i>chrmnum)
        {
            //std::cout<<"() operator exceed the range of index!"<<std::endl;
            exit(0);
        }
        return chrm[i];
    }
    bool operator==(const Chrom& chrom)
    {
        int tag = 0;
        if(chrmnum==chrom.chrmnum)
        {
            tag = 1;
        }
        else
        {
            return 0;
        }
        int i = 0;
        for(auto eachbin=chrm.begin(); eachbin!=chrm.end(); ++i, ++eachbin)
        {
            if(*eachbin==chrom.chrm[i])
            {
                tag++;
            }
        }
        return tag==1+chrmnum;
    }
    void Show()
    {
        std::cout<<"Bits of chrm: "<<chrmnum<<std::endl;
        std::vector<double> value = getValue();
        for(auto i=0; i<chrmnum; ++i)
        {
            std::cout<<"bit: "<<i<<"\t"<<chrm[i].getbit()<<"\t"\
                     <<chrm[i].getvalue()<<"\t"<< std::setprecision(7)\
                     <<value[i]<<"\t"\
                     <<chrm[i].getstring()<<std::endl;
        }
    }
    void test_Chrom()
    {
        int _chrmnum = 3;
        std::vector<int> _bitlist = {3, 4, 5};
        Chrom tchrm1;
        Binary bin1("010");
        Binary bin2("0110");
        Binary bin3("10001");
        tchrm1.Insert(bin1);
        tchrm1.Insert(bin2);
        tchrm1.Insert(bin3);
        if(tchrm1.getChrmNum()!=3)
        {
            //std::cout<<"F";
        }
        else
        {
            //std::cout<<".";
        }
        Chrom tchrm2(tchrm1);
        if(!(tchrm1==tchrm2))
        {
            //std::cout<<"F";
        }
        else
        {
            //std::cout<<".";
        }
        std::vector<double> values = tchrm1.getValue();
        std::vector<double> objvalues = {2.0/7.0, 6.0/15.0, 9.0/31.0};
        int i = 0;
        for(auto eachv=values.begin(); eachv!=values.end(); ++eachv, ++i)
        {
            if(abs(*eachv-objvalues[i])<1.0e-20)
            {
                //std::cout<<".";
            }
            else
            {
                //std::cout<<"F";
            }
        }
        //std::cout<<std::endl;
    }
};

class Group
{
private:
    int GroupNum;
public:
    static int ChrmNum;
    static std::vector<int> bitlist;
    static std::vector< std::pair<double, double> > limits;
    
    std::vector<Chrom> Members;
    
    Group()
    {
        GroupNum=0;
    }
    Group(int _groupnum, int _chrmnum,
          std::vector<int> _bitlist,
          std::vector< std::pair<double, double> > _limits
    )
    {
        Initialize(_groupnum, _chrmnum, _bitlist, _limits);
    }
    Group(const Group &group)
    {
        GroupNum = 0;
        Members.clear();
        //std::cout<<"inside 151"<<std::endl;
        for(auto member=group.Members.begin(); member!=group.Members.end(); ++member)
        {
            Insert(Chrom(*member));
        }
        //std::cout<<"inside 152"<<std::endl;
    }
    void Show()
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
    void setGroupNum(int _num) {GroupNum = _num;}
    static void setChrmNum(int _num) {ChrmNum = _num;}
    static void setBitList(std::vector<int> _bits)
    {   
        bitlist.clear();
        for(auto bit=_bits.begin(); bit!=_bits.end(); ++bit)
        {
            bitlist.push_back(*bit);
        }
    }
    static void setLimits(std::vector< std::pair<double, double> > _limits)
    {
        limits.clear();
        for(auto eachpair=_limits.begin(); eachpair!=_limits.end(); ++eachpair)
        {
            limits.push_back(std::make_pair(eachpair->first, eachpair->second));
        }
    }
    void Initialize(int _groupnum, int _chrmnum,
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
            Insert(Chrom(ChrmNum, bitlist));
        }
    }
    void Insert(Chrom chrom)
    {
        GroupNum += 1;
        Members.push_back(chrom);
    }
    void operator+=(const Group& group)
    {
        for(auto member=group.Members.begin(); member!=group.Members.end(); ++member)
        {
            this->Insert(*member);
        }
    }
    Group newGroup(int _groupnum)
    {
        Group newgroup;
        for(auto i=0; i<_groupnum; ++i)
        {
            newgroup.Insert(Chrom(ChrmNum, bitlist));
        }
        return newgroup;
    }
    Group crossOverTwo(Chrom chrm1, Chrom chrm2)
    {
        Chrom newchrm1;
        Chrom newchrm2;
        for(int i=0; i<ChrmNum; i++)
        {
            Binary bin1(chrm1(i));
            Binary bin2(chrm2(i));
            int bit = bin1.getbit();            
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
    Group crossOverGroup(Group &pgroup)
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
    Chrom mutateOne(Chrom chrm, int mutatemode)
    {
        int chrmnum = chrm.getChrmNum();
        Chrom newchrm;
        for(int ci = 0; ci<chrmnum; ++ci)
        {
            std::string binstr = chrm(ci).getstring();
            int bit = chrm(ci).getbit();
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
    Group mutateGroup(Group &pgroup, double Mutaterate)
    {
        int gn = pgroup.GroupNum;
        Group mgroup;
        for(int gi=0; gi<gn; gi++)
        {
            double mrate = rand()%1000/1000.0;
            //std::cout<<"check mutate rate: "<<mrate<<std::endl;
            Chrom tmp;
            if(mrate<Mutaterate)
            {
                tmp = mutateOne(pgroup.Members[gi], 1);
            }
            else
            {
                tmp = pgroup.Members[gi];
            }
            mgroup.Insert(Chrom(tmp));
        }
        return mgroup;
    }
    
    std::vector<double> calculateFitness(Group& pgroup, double (*p_fitness)(std::vector<double> vars))
    {
        //std::cout<<"break calculateFitness 0"<<std::endl;
	    std::vector<double> fitness;
	    int gn = pgroup.GroupNum;
	    //std::cout<<"groupnum: "<<gn<<std::endl;
	    for(int ci = 0; ci<gn; ++ci)
	    {
	        
		    std::vector<double> vars;
		    std::vector<double> values = pgroup.Members[ci].getValue();
		    //std::cout<<"break calculateFitness 1 at "<<ci<<std::endl;
		    int length = pgroup.ChrmNum;
		    //std::cout<<"chrmnum: "<<length<<std::endl;
		    //std::cout<<"in Clac fitness!"<<std::endl;
		    for(int i=0; i<length; i++)
		    {
			    vars.push_back(limits[i].first+(limits[i].second - limits[i].first)*values[i]);
			    //std::cout<<vars[i]<<" ";
			    
		    }
		    //std::cout<<p_fitness(vars)<<std::endl;
		    fitness.push_back(p_fitness(vars));
	    }
	    //std::cout<<"break calculateFitness end"<<std::endl;
	    return fitness;
    }
    
    
    Group Selection(Group pgroup, double (*p_fitness)(std::vector<double> vars), int selectedsize)
    {
        //std::cout<<"break 100: "<<selectedsize<<std::endl;
        //std::cout<<"break 10"<<std::endl;
        //pgroup.Show();
        //std::cout<<"break 11"<<std::endl;
        std::vector<double> fitness = Group::calculateFitness(pgroup, p_fitness);
        //std::cout<<"break 12"<<std::endl;
        int gn = pgroup.GroupNum;
        
        double fitmin, fitmax;
        fitmin = fitness[0];
        fitmax = fitness[0];
        for(int i=0; i<gn; i++)
        {
            //std::cout<<fitness[i]<<std::endl;
            if(fitmin>fitness[i])
            {
                fitmin = fitness[i];
            }
            if(fitmax<fitness[i])
            {
                fitmax = fitness[i];
            }
        }
        //std::cout<<"break 13"<<std::endl;
        std::vector<double> ufitness;
        std::vector<double> afitness;
        //unified the fitness array and accumulated array
        double tmp = 0;
        for(int i=0; i<gn; i++)
        {
            double t = (fitness[i]-fitmin)/(fitmax-fitmin+1.0e-20);
            ufitness.push_back(t);
            tmp += t;
            //std::cout<<i<<" "<<t<<std::endl;
        }
        double ttmp = 0;
        for(int i=0; i<gn; i++)
        {   
            ttmp += ufitness[i];
            afitness.push_back(ttmp/tmp);
        }
        //std::cout<<"break 14"<<std::endl;
        //std::cout<<"check unite fitness!"<<std::endl;
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
            //std::cout<<"random number: "<<rannum<<std::endl;
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
            //std::cout<<"check selected index: "<<selectednum<<" "<<ufitness[selectednum]<<std::endl;
            sgroup.Insert(Chrom(pgroup.Members[selectednum]));
        }
        //sgroup.Show();
        //std::cout<<"break 15"<<std::endl;
        return sgroup;
    }
    Chrom bestOne(Group pgroup, double (*p_fitness)(std::vector<double> vars))
    {   
        //pgroup.Show();
        //std::cout<<"break bestOne 0"<<std::endl;
   	    std::vector<double> fitness = Group::calculateFitness(pgroup, p_fitness);
   	    //std::cout<<"break bestOne 1"<<std::endl;
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
   	    //std::cout<<"break bestOne 2"<<std::endl;
   	    return Chrom(pgroup.Members[tag]);
   	
    }
    double bestFitness(Group pgroup, double (*p_fitness)(std::vector<double> vars))
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
            //std::cout<<"Member: "<<i<<std::endl;
            //std::cout<<"Bits of chrm: "<<ChrmNum<<std::endl;
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
                //std::cout<<"bit: "<<i<<"\t"<<bitlist[i]<<"\t"\
                     <<"\t"<< std::setprecision(7)\
                     <<values[i]<<"\t"<<fvalues[i]<<"\t"\
                     <<std::endl;
            }
            //std::cout<<"fitness: "<<fitness[i]<<std::endl;
        }
        std::cout<<"best fitness is at member "<<tag<<std::endl;
   	    for(auto var=bestvars.begin(); var!=bestvars.end(); ++var)
   	    {
   	        std::cout<<*var<<" ";
   	    }
   	    std::cout<<std::endl;
   	    return tmp;
   	
    }
    Chrom simpleGA(int times, double (*p_fitness)(std::vector<double> vars))
    {
    	double MutateRate = 0.01;
    	double selectedrate = 0.5;
    	//initialize the parents group
    	Group Parents = Group::newGroup(GroupNum);
    	//loop
    	//std::cout<<"break simpleGA before loop!"<<std::endl;
    	for(int tstep=0; tstep<times; ++tstep)
    	{
    		std::cout<<"\n\n"<<tstep<<std::endl;
    		//std::cout<<"GroupNum: "<<GroupNum<<std::endl;
    		Group Selecteds = Group::Selection(Parents, p_fitness, GroupNum);
    		//std::cout<<"check selected number: "<<Selecteds.GroupNum<<std::endl;
    		//std::cout<<"break simpleGA 1"<<std::endl;
    		Group OffSpring = Group::crossOverGroup(Selecteds);
    		//std::cout<<"check offspring number: "<<OffSpring.GroupNum<<std::endl;
    		//std::cout<<"break simpleGA 2"<<std::endl;
    		Group MutatedGroup = Group::mutateGroup(OffSpring, MutateRate);
    		//std::cout<<"check MutatedGroup number: "<<MutatedGroup.GroupNum<<std::endl;
    		//std::cout<<"break simpleGA 3"<<std::endl;
    		//Parents.Show();
    		Chrom Best = Group::bestOne(Parents, p_fitness);
    		//std::cout<<"break simpleGA 4"<<std::endl;
    		std::cout<<"best member of generation "<<tstep<<" is:"<<std::endl;
    		Best.Show();
    		MutatedGroup += Parents;
    		//std::cout<<"check MutatedGroup number: "<<MutatedGroup.GroupNum<<std::endl;
    		//std::cout<<"break simpleGA 6"<<std::endl;
    		Parents = Group::Selection(MutatedGroup, p_fitness, selectedrate*GroupNum);
    		//std::cout<<"check 1parents number: "<<Parents.GroupNum<<std::endl;
    		//std::cout<<"break simpleGA 7"<<std::endl;
    		Parents.Insert(Best);
    		//std::cout<<"check 2parents number: "<<Parents.GroupNum<<std::endl;
    		int newnum = GroupNum - Parents.GroupNum;
    		Group tmpGroup = Group::newGroup(newnum);
    		//std::cout<<"check tmpgroup number: "<<tmpGroup.GroupNum<<std::endl;
    		Parents += tmpGroup;
    		//std::cout<<"check 3parents number: "<<Parents.GroupNum<<std::endl;
    	}
    	
    	bestFitness(Parents, p_fitness);
    	return Group::bestOne(Parents, p_fitness);
    }
};
std::vector< int > Group::bitlist;
std::vector< std::pair<double, double> > Group::limits;
int Group::ChrmNum;
#endif

