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
#include <vector>
#include <cstdlib>
#include <time.h> 
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm> 
#include <string.h>
using namespace std;

stringstream outbufer;

class binary
{
private:
	string binstr;
public:
	binary(){};
	binary(string binstr_) {binstr=binstr_;}
	void _randombinary(int bits);
	string randombinary(int bits);
	void set(string str){binstr=str;}
	string get(){return binstr;}
	int getbits() {return binstr.size();}
	long long encodebinary(string _binstr);
	string codenumber(long long number);
	void exchange(string *str1, string *str2, int pos);
	void exchange(binary *str1, binary *str2, int pos);
	string substitute(string str, string substr, int pos);
	string minimum(int bits);
	string maximum(int bits);
	
};

void binary::_randombinary(int bits)
{
	binstr = "";
	for(int i=0; i<bits; i++)
	{
		string tmp;
		outbufer<<rand()%2;
		outbufer>>tmp;
		outbufer.clear();
		binstr += tmp;
		//cout<<random()%2<<endl;
	}
}

string binary::randombinary(int bits)
{
	
	string chrn = "";
	for(int i=0; i<bits; i++)
	{
		string tmp;
		outbufer<<rand()%2;
		outbufer>>tmp;
		outbufer.clear();
		chrn += tmp;
		//cout<<random()%2<<endl;
	}
	return chrn;
}

long long binary::encodebinary(string _binstr)
{
	long long number = 0;
	for(int i=_binstr.size()-1; i>=0; i--)
	{
		int tmp;
		outbufer<<_binstr[i];
		outbufer>>tmp;
		outbufer.clear();
		number += tmp*pow(2, _binstr.size()-i-1);
	}
	return number;
}

string binary::codenumber(long long number)
{
	long long tmp = number;
	string _binstr = "";
	while(tmp!=0)
	{
		long long temp = tmp%2;
		string ttmp;
		tmp = (tmp-temp)/2;
		outbufer<<temp;
		outbufer>>ttmp;
		outbufer.clear();
		_binstr = ttmp + _binstr;
	}
	return _binstr;
}

void binary::exchange(string *str1, string *str2, int pos)
{
	string tmp;
	string tmpstr1(*str1, 0, pos);
	int len = str1->size();
	tmp.assign(*str2, pos, len-pos);
	tmpstr1 += tmp;
	
	string tmpstr2(*str2, 0, pos);
	tmp = "";
	tmp.assign(*str1, pos, len-pos);
	tmpstr2 += tmp;
	*str1 = tmpstr1;
	*str2 = tmpstr2;
	//cout<<*str1<<"\t"<<*str2<<endl;
}

void binary::exchange(binary *bin1, binary *bin2, int pos)
{
	string cstr1 = bin1->binstr;
	string cstr2 = bin2->binstr;
	bin1->exchange(&cstr1, &cstr2, pos);
	bin1->set(cstr1);
	bin2->set(cstr2);
}

string binary::substitute(string str, string substr, int pos)
{
	str = str.replace(pos, substr.size(), substr.c_str());
	return str;
}

string binary::minimum(int bits)
{
	string chrn = "";
	for(int i=0; i<bits; i++)
	{
		chrn += "0";
	}
	return chrn;
}

string binary::maximum(int bits)
{
	string chrn = "";
	for(int i=0; i<bits; i++)
	{
		chrn += "1";
	}
	return chrn;
}

class Chrom
{
private:
	static vector< pair<double, double> > limits;
	static vector<double> range;
public:
	int chrmnum;
	vector< binary > chrm;
	vector< int > bitslist;
	vector< int > fixed;
	Chrom(){};
	void SetChrmNum(int _chrmnum);
	int GetChrmNum() {return chrm.size();}
	void SetFixedArray(int *_fixed);
	void SetBitsArray(int *_bitslist);
	void SetBitsArray(vector<int> _bitslist);
	void RandomConstructChrm(int *_bits);
	void RandomConstructChrm();
	void DefaultFixedArray();
	static void SetLimits(vector< pair<double, double> > limits_);
	static void ShowLimits();
	vector<double> GetValue();
	vector<Chrom> NewGroup(int groupnum, int varnum, vector<int> bitslist_);
	vector<Chrom> CrossOverIndividual(Chrom chrm1, Chrom chrm2, double CrossRate);
	vector<Chrom> CrossOverGroup(vector<Chrom> group, double CrossRate);
	Chrom MutateIndividual(Chrom chrms, int mutatemode, double Mutaterate);
	vector< Chrom > MutateGroup(vector< Chrom > chrms, double Mutaterate);
	vector<Chrom> Selection(vector<Chrom> chrms, double (*p_fitness)(vector<double> vars), int selectedsize);
	vector<double> CalculateFitness(vector<Chrom> chrms, double (*p_fitness)(vector<double> vars));
	void Show();
	Chrom SingleObjectEvolution(int times, int groupnum, int varnum, vector<int> bitslist_, vector< pair<double, double> > limits_, double (*p_fitness)(vector<double> vars));
	Chrom BestIndividual(vector<Chrom> Group, double (*p_fitness)(vector<double> vars));
	double BestFitness(vector<Chrom> Group, double (*p_fitness)(vector<double> vars));
	Chrom SingleObjectEvolution(double minfitness, int groupnum, int varnum, vector<int> bitslist_, vector< pair<double, double> > limits_, double (*p_fitness)(vector<double> vars), int MaxGenerations);
	
};

vector< pair<double, double> > Chrom::limits;
vector< double > Chrom::range;

//always call this function before generating any chromsomes.
void Chrom::SetChrmNum(int _chrmnum)
{
	chrmnum = _chrmnum;
	chrm.resize(chrmnum);
	bitslist.resize(chrmnum);
	fixed.resize(chrmnum);
	
	DefaultFixedArray();
}

void Chrom::SetBitsArray(int *_bitslist)
{
	std::vector< int >::iterator iter;
	iter = bitslist.begin();
	for(int i=0; iter<bitslist.end(); i++, iter++)
	{
		*iter = _bitslist[i];
	}
}

void Chrom::SetBitsArray(vector<int> _bitslist)
{
	std::vector< int >::iterator iter;
	iter = bitslist.begin();
	for(int i=0; iter<bitslist.end(); i++, iter++)
	{
		*iter = _bitslist[i];
	}
}

void Chrom::SetFixedArray(int *_fixed)
{
	std::vector< int >::iterator iter;
	iter = fixed.begin();
	for(int i=0; iter<fixed.end(); i++, iter++)
	{
		*iter = _fixed[i];
	}
}

void Chrom::DefaultFixedArray()
{
	std::vector< int >::iterator iter;
	iter = fixed.begin();
	for(; iter<fixed.end(); iter++)
	{
		*iter = 1;
	}
}

void Chrom::SetLimits(vector< pair<double, double> > limits_)
{
	int chrmsize = limits_.size();
	for(int i=0; i<chrmsize; i++)
	{
		pair<double, double> tmp = make_pair(limits_[i].first, limits_[i].second);
		range.push_back(limits_[i].second-limits_[i].first);
		limits.push_back(tmp);
	}
}

void Chrom::ShowLimits()
{
	int chrmsize = limits.size();
	for(int i=0; i<chrmsize; i++)
	{
		cout<<limits[i].first<<" "<<limits[i].second<<endl;
	}
}

void Chrom::RandomConstructChrm(int *_bits)
{
	std::vector< binary >::iterator iter;
	iter = chrm.begin();
	for(int i=0; iter<chrm.end(); i++, iter++)
	{
		if(fixed[i])
		{
			iter->_randombinary(_bits[i]);
		}
		else
		{
			string tmp = iter->minimum(_bits[i]);
			iter->set(tmp);
		}
	}
}

//before calling this function, the bits array must be initialize.
void Chrom::RandomConstructChrm()
{
	std::vector< binary >::iterator iter;
	iter = chrm.begin();
	for(int i=0; iter<chrm.end(); i++, iter++)
	{
		//fixed means whether or not a chrm can be changed. 
		//if such a chrm is fixed, then it will always be set as the binary string of zero.
		if(fixed[i])
		{
			iter->_randombinary(bitslist[i]);
		}
		else
		{
			string tmp = iter->minimum(bitslist[i]);
			iter->set(tmp);
		}
	}
}

vector< double > Chrom::GetValue()
{
	std::vector< double > result;
	std::vector< binary >::iterator iter;
	iter = chrm.begin();
	for(int i=0; iter<chrm.end(); i++, iter++)
	{
		//fixed means whether or not a chrm can be changed. 
		//if such a chrm is fixed, then it will always be set as the binary string of zero.
		result.push_back(iter->encodebinary(iter->get())/pow(2, iter->getbits()));
	}
	return result;
}

vector< Chrom > Chrom::CrossOverIndividual(Chrom chrm1, Chrom chrm2, double CrossRate = 0.9)
{
	int chrmnum = chrm1.GetChrmNum();
	//cout<<"num"<<chrmnum<<endl;
	Chrom newchrm1, newchrm2;
	newchrm1.SetChrmNum(chrmnum);
	newchrm2.SetChrmNum(chrmnum);
	//cout<<newchrm1.chrm.size()<<endl;
	chrm1.SetChrmNum(chrmnum);
	for(int i=0; i<chrmnum; i++)
	{
		string cstr1 = chrm1.chrm[i].get();
		string cstr2 = chrm2.chrm[i].get();
		int bits = chrm1.chrm[i].getbits();
		//generate a random number 
		//which determine the position where happen the crossover.
		int crosspos = rand()%bits;
		chrm1.chrm[i].exchange(&cstr1, &cstr2, crosspos);
		binary bin1(cstr1);
		binary bin2(cstr2);
		newchrm1.chrm[i] = bin1;
		newchrm2.chrm[i] = bin2;
	}
	//newchrm1.Show();
	vector< Chrom > tmp;
	tmp.push_back(newchrm1);
	tmp.push_back(newchrm2);
	return tmp;
}

vector< Chrom > Chrom::CrossOverGroup(vector< Chrom > Group, double CrossRate = 0.9)
{
	vector< Chrom > OffSpring;
	int groupsize = Group.size();
	random_shuffle(Group.begin(), Group.end());
	for(int i=0; i<groupsize/2; i++)
	{
		vector< Chrom > tmp = CrossOverIndividual(Group[i], Group[i+1]);
		OffSpring.push_back(tmp[0]);
		OffSpring.push_back(tmp[1]);
	}
	return OffSpring;
}

Chrom Chrom::MutateIndividual(Chrom chrms, int mutatemode=1, double Mutaterate=0.1)
{
	string zero("0");
	string one("1");
	int chrmnum = chrms.GetChrmNum();
	Chrom newchrm;
	newchrm.SetChrmNum(chrmnum);
	for(int ci = 0; ci<chrmnum; ci++)
	{
		string binstr = chrms.chrm[ci].get();
		int bits = chrms.chrm[ci].getbits();
		for(int mi = 0; mi<mutatemode; mi++)
		{
			int mutatepos = rand()%bits;
			char* slice = &binstr[mutatepos];
			if(strcmp(slice, "1")==0)
			{
				binstr.replace(mutatepos, 1, "0");
			}
			else
			{
				binstr.replace(mutatepos, 1, "1");
			}
			
		}
		binary bin1(binstr);
		newchrm.chrm[ci] = bin1;
	}
	return newchrm;
}

vector< Chrom > Chrom::MutateGroup(vector< Chrom > Group, double Mutaterate=0.1)
{	
	//cout<<"watch MutateGroup 0"<<endl;
	int groupsize = Group.size();
	vector< Chrom > MutatedGroup;
	for(int gi=0; gi<groupsize; gi++)
	{
		double mrate = rand()%1000/1000.0;
		Chrom tmp;
		if(mrate<Mutaterate)
		{
			tmp = MutateIndividual(Group[gi]);
		}
		else
		{
			tmp = Group[gi];
		}
		MutatedGroup.push_back(tmp);
	}
	//cout<<"watch MutateGroup 1"<<endl;
	return MutatedGroup;
}

vector< Chrom > Chrom::Selection(vector<Chrom> chrms, double (*p_fitness)(vector<double>), int selectednum=-1)
{
	vector<double> fitness = Chrom::CalculateFitness(chrms, p_fitness);
	//ordertype=0 means that the larger value, the better fitness.
	int groupsize = chrms.size();
	int selectedsize;
	if(selectednum==-1)
	{
		selectedsize = chrms.size();
	}
	else
	{
		selectedsize = selectednum;
	}
	double fitmin, fitmax;
	fitmin = fitness[0];
	fitmax = fitness[0];
	for(int i=0; i<groupsize; i++)
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
	
	vector<double> ufitness(fitness);
	vector<double> afitness;
	//unified the fitness array and accumulated array
	double tmp = 0;
	for(int i=0; i<groupsize; i++)
	{
		ufitness[i] = (fitness[i]-fitmin)/(fitmax-fitmin);
		tmp += ufitness[i];
		afitness.push_back(tmp);
	}
	
	//roulette
	vector< Chrom > newchrms;
	for(int i=0; i<selectedsize; i++)
	{
		int rannum = rand()%10000/10000.0;
		int selectednum = 0;
		for(int j=0; j<groupsize; j++)
		{
			if(rannum<=afitness[j])
			{
				selectednum = j;
				break;
			}
		}
		newchrms.push_back(chrms[selectednum]);
	}
	return newchrms;
}

vector<double> Chrom::CalculateFitness(vector<Chrom> chrms, double (*p_fitness)(vector<double> vars))
{
	vector<double> fitness;
	int groupsize = chrms.size();
	for(int eachg = 0; eachg<groupsize; eachg++)
	{
		vector<double> vars;
		Chrom eachchrm(chrms[eachg]);
		vector<double> values = eachchrm.GetValue();
		int length = eachchrm.chrm.size();
		for(int i=0; i<length; i++)
		{
			vars.push_back(limits[i].first+range[i]*values[i]);
		}
		fitness.push_back(p_fitness(vars));
	}
	return fitness;
}

void Chrom::Show()
{
	vector<double> values = GetValue();
	std::vector< binary >::iterator iter;
	iter = chrm.begin();
	for(int i=0; iter<chrm.end(); i++, iter++)
	{
		cout<<"chrm "<<i<<": "<<iter->get()<<", "<<iter->encodebinary(iter->get())<<", "<<values[i]<<endl;
	}
}

vector<Chrom> Chrom::NewGroup(int groupnum, int varnum, vector<int> bitslist_)
{
	vector< Chrom > Group;
	for(int gi=0; gi<groupnum; gi++)
	{
		Chrom chrmtmp;
		chrmtmp.SetChrmNum(varnum);
		chrmtmp.SetBitsArray(bitslist_);
		chrmtmp.RandomConstructChrm();
		Group.push_back(chrmtmp);
	}
	return Group;
}

Chrom Chrom::SingleObjectEvolution(int times, int groupnum, int varnum, vector<int> bitslist_, vector< pair<double, double> > limits_, double (*p_fitness)(vector<double> vars))
{
	double SelectedRate = 0.8;
	double CrossOverRate = 0.8;
	double MutateRate = 0.01;
	
	//initialize the parents group
	vector< Chrom > Parents = Chrom::NewGroup(groupnum, varnum, bitslist_);
	Parents[0].SetLimits(limits_);
	//loop
	for(int timestep=0; timestep<times; timestep++)
	{
		cout<<timestep<<endl;
		//Chrom::Selection(vector<Chrom> chrms, vector<double> fitness, int ordertype=0)
		vector<Chrom> Selecteds = Chrom::Selection(Parents, p_fitness);
		Selecteds[0].Show();
		vector< Chrom > OffSpring = Chrom::CrossOverGroup(Selecteds);
		//cout<<"watch offspring"<<endl;
		OffSpring[0].Show();
		OffSpring[9].Show();
		//cout<<"watch offspring"<<endl;
		vector< Chrom > MutatedGroup = Chrom::MutateGroup(OffSpring, 0.01);
		
		MutatedGroup[0].Show();
		MutatedGroup[1].Show();
		MutatedGroup[9].Show();
		//cout<<"watch mutated"<<endl;
		//cout<<"watch point 1"<<endl;
		//NewGroup.insert(NewGroup.end(), MutatedGroup.begin(), MutatedGroup.end());
		//cout<<"watch point 2"<<endl;
		Chrom Best = Chrom::BestIndividual(Parents, p_fitness);
		Parents = Chrom::Selection(MutatedGroup, p_fitness, 0.8*Parents.size());
		Parents.push_back(Best);
		int tmpsize = Parents.size();
		vector<Chrom> tmpGroup = Chrom::NewGroup(groupnum-tmpsize, varnum, bitslist_);
		Parents.insert(Parents.end(), tmpGroup.begin(), tmpGroup.end());
		//cout<<"watch point 3"<<endl;
	}
	//cout<<"watch point 4"<<endl;
	return Chrom::BestIndividual(Parents, p_fitness);
}


Chrom Chrom::SingleObjectEvolution(double minfitness, int groupnum, int varnum, vector<int> bitslist_, vector< pair<double, double> > limits_, double (*p_fitness)(vector<double> vars), int MaxGenerations=10000)
{
	double SelectedRate = 0.8;
	double CrossOverRate = 0.8;
	double MutateRate = 0.01;
	
	//initialize the parents group
	vector< Chrom > Parents = Chrom::NewGroup(groupnum, varnum, bitslist_);
	Parents[0].SetLimits(limits_);
	double bestfitness = Chrom::BestFitness(Parents, p_fitness);
	//loop
	int timestep = 0;
	while(bestfitness>minfitness && timestep<MaxGenerations)
	{
		timestep++;
		cout<<timestep<<endl;
		//Chrom::Selection(vector<Chrom> chrms, vector<double> fitness, int ordertype=0)
		vector<Chrom> Selecteds = Chrom::Selection(Parents, p_fitness);
		Selecteds[0].Show();
		vector< Chrom > OffSpring = Chrom::CrossOverGroup(Selecteds);
		//cout<<"watch offspring"<<endl;
		OffSpring[0].Show();
		OffSpring[9].Show();
		//cout<<"watch offspring"<<endl;
		vector< Chrom > MutatedGroup = Chrom::MutateGroup(OffSpring, 0.01);
		
		MutatedGroup[0].Show();
		MutatedGroup[1].Show();
		MutatedGroup[9].Show();
		//cout<<"watch mutated"<<endl;
		//cout<<"watch point 1"<<endl;
		//NewGroup.insert(NewGroup.end(), MutatedGroup.begin(), MutatedGroup.end());
		//cout<<"watch point 2"<<endl;
		Chrom Best = Chrom::BestIndividual(Parents, p_fitness);
		Parents = Chrom::Selection(MutatedGroup, p_fitness, 0.8*Parents.size());
		Parents.push_back(Best);
		int tmpsize = Parents.size();
		vector<Chrom> tmpGroup = Chrom::NewGroup(groupnum-tmpsize, varnum, bitslist_);
		Parents.insert(Parents.end(), tmpGroup.begin(), tmpGroup.end());
		bestfitness = Chrom::BestFitness(Parents, p_fitness);
		//cout<<"watch point 3"<<endl;
	}
	//cout<<"watch point 4"<<endl;
	return Chrom::BestIndividual(Parents, p_fitness);
}

Chrom Chrom::BestIndividual(vector<Chrom> Group, double (*p_fitness)(vector<double> vars))
{
	vector<double> fitness = Chrom::CalculateFitness(Group, p_fitness);
	int tag = 0;
	int groupsize = Group.size();
	double tmp = fitness[0];
	for(int i=0; i<groupsize; i++)
	{
		if(tmp>fitness[i])
		{
			tmp = fitness[i];
			tag = i;
		}
	}
	cout<<"Best fitness:"<<fitness[tag]<<endl;
	return Group[tag];
	
}

double Chrom::BestFitness(vector<Chrom> Group, double (*p_fitness)(vector<double> vars))
{
		vector<double> fitness = Chrom::CalculateFitness(Group, p_fitness);
	int tag = 0;
	int groupsize = Group.size();
	double tmp = fitness[0];
	for(int i=0; i<groupsize; i++)
	{
		if(tmp>fitness[i])
		{
			tmp = fitness[i];
			tag = i;
		}
	}
	cout<<"Best fitness:"<<fitness[tag]<<endl;
	return fitness[tag];
}

double fitness_test(vector<double> vars)
{
	double p1, p2, p3;
	p1 = vars[0]; p2 = vars[1]; p3 = vars[2];
	return p1*p1+p2*p2+p3*p3;
}

int test()
{
	srand((unsigned)time(NULL));
	Chrom tmp;
	double low[3] = {-100, -100, -100};
	double up[3] = {100, 100, 100};
	vector< pair<double, double> > limits_;
	for(int i=0; i<3; i++)
	{
		limits_.push_back(make_pair(low[i], up[i]));
	}
	
	double (*p_fitness)(vector<double>);
	p_fitness = fitness_test;
	vector<int> bitslist;
	bitslist.push_back(15); bitslist.push_back(15); bitslist.push_back(15);
	Chrom chrm = tmp.SingleObjectEvolution(10, 1000, 3, bitslist, limits_, p_fitness, 10000);
	chrm.Show();
	return 1;
}
