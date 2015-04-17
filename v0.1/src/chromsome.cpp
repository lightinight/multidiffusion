#include "chromsome.h"

using namespace std;

//always call this function before generating any chromsomes.
void Chrom::setChrmNum(int _chrmnum)
{
	chrmnum = _chrmnum;
	chrm.resize(chrmnum);
	limits.resize(chrmnum);
	bitslist.resize(chrmnum);
}

void Chrom::setBitsList(vector<int> _bitslist)
{
	vector< int >::iterator iter;
	iter = bitslist.begin();
	for(int i=0; iter<bitslist.end(); i++, iter++)
	{
		*iter = _bitslist[i];
	}
}

void Chrom::setLimits(vector< pair<double, double> > _limits)
{
	int chrmsize = _limits.size();
	for(int i=0; i<chrmsize; i++)
	{
		limits[i] = make_pair(_limits[i].first, _limits[i].second);
	}
}

void Chrom::showLimits()
{
	int chrmsize = limits.size();
	for(int i=0; i<chrmsize; i++)
	{
		cout<<limits[i].first<<" "<<limits[i].second<<endl;
	}
}

void Chrom::randomConstructChrm()
{
	vector< binary >::iterator iter;
	iter = chrm.begin();
	for(int i=0; iter<chrm.end(); i++, iter++)
	{
		iter->set(iter->randombinary(bitslist[i]));
	}
}

vector< double > Chrom::getValue()
{
	vector< double > result;
	vector< binary >::iterator iter;
	iter = chrm.begin();
	for(int i=0; iter<chrm.end(); i++, iter++)
	{
		result.push_back(iter->decode(iter->get())/pow(2, iter->getbits()));
	}
	return result;
}

vector< Chrom > Chrom::crossOverAB(Chrom chrm1, Chrom chrm2)
{
	int chrmnum = chrm1.getChrmNum();
	Chrom newchrm1, newchrm2;
	newchrm1.setChrmNum(chrmnum);
	newchrm2.setChrmNum(chrmnum);
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
	vector< Chrom > tmp;
	tmp.push_back(newchrm1);
	tmp.push_back(newchrm2);
	return tmp;
}

vector< Chrom > Chrom::crossOverGroup(vector< Chrom > Group)
{
	vector< Chrom > OffSpring;
	int groupsize = Group.size();
	random_shuffle(Group.begin(), Group.end());
	for(int i=0; i<groupsize/2; i++)
	{
		vector< Chrom > tmp = crossOverAB(Group[i], Group[i+1]);
		OffSpring.push_back(tmp[0]);
		OffSpring.push_back(tmp[1]);
	}
	return OffSpring;
}

Chrom Chrom::mutateA(Chrom chrms, int mutatetimes)
{
	string zero("0");
	string one("1");
	int chrmnum = chrms.getChrmNum();
	Chrom newchrm;
	newchrm.setChrmNum(chrmnum);
	for(int ci = 0; ci<chrmnum; ci++)
	{
		string binstr = chrms.chrm[ci].get();
		int bits = chrms.chrm[ci].getbits();
		for(int mi = 0; mi<mutatetimes; mi++)
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

vector< Chrom > Chrom::mutateGroup(vector< Chrom > Group, double Mutaterate, int mutatepoints)
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
			tmp = mutateA(Group[gi], mutatepoints);
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

void Chrom::show()
{
	vector<double> values = getValue();
	vector< binary >::iterator iter;
	iter = chrm.begin();
	for(int i=0; iter<chrm.end(); i++, iter++)
	{
		cout<<"chrm "<<i<<": "<<iter->get()<<", "<<iter->decode(iter->get())<<", "<<values[i]<<endl;
	}
}

vector< Chrom > Chrom::selection(vector<Chrom> chrms, double (*p_fitness)(vector<double>), int selectednum)
{
	vector<double> fitness = Chrom::calculateFitness(chrms, p_fitness);
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

vector<Chrom> Chrom::newGroup(int groupnum)
{
	vector< Chrom > Group;
	for(int gi=0; gi<groupnum; gi++)
	{
		Chrom chrmtmp;
		chrmtmp.setChrmNum(chrmnum);
		chrmtmp.setBitsList(bitslist);
		chrmtmp.setLimits(limits);
		chrmtmp.randomConstructChrm();
		Group.push_back(chrmtmp);
	}
	return Group;
}

Chrom Chrom::singleObjectEvolution(int times, int groupnum, double (*p_fitness)(vector<double> vars))
{
	double SelectedRate = 0.8;
	double CrossOverRate = 0.8;
	double MutateRate = 0.01;
	int mutatepoints = 2;
	//initialize the parents group
	vector< Chrom > Parents = Chrom::newGroup(groupnum);
	Parents[0].setLimits(limits);
	for(int timestep=0; timestep<times; timestep++)
	{
		cout<<timestep<<endl;
		vector<Chrom> Selecteds = Chrom::selection(Parents, p_fitness, -1);
		vector< Chrom > OffSpring = Chrom::crossOverGroup(Selecteds);
		vector< Chrom > MutatedGroup = Chrom::mutateGroup(OffSpring, MutateRate, mutatepoints);
		
		Chrom Best = Chrom::bestIndividual(Parents, p_fitness);
		Parents = Chrom::selection(MutatedGroup, p_fitness, SelectedRate*Parents.size());
		Parents.push_back(Best);
		int tmpsize = Parents.size();
		vector<Chrom> tmpGroup = Chrom::newGroup(groupnum-tmpsize);
		Parents.insert(Parents.end(), tmpGroup.begin(), tmpGroup.end());
	}
	return Chrom::bestIndividual(Parents, p_fitness);
}

vector<double> Chrom::calculateFitness(vector<Chrom> chrms, double (*p_fitness)(vector<double> vars))
{
	vector<double> fitness;
	int groupsize = chrms.size();
	for(int eachg = 0; eachg<groupsize; eachg++)
	{
		vector<double> vars;
		Chrom eachchrm(chrms[eachg]);
		vector<double> values = eachchrm.getValue();
		int length = eachchrm.chrm.size();
		for(int i=0; i<length; i++)
		{
		    double range =  limits[i].second - limits[i].first;
			vars.push_back(limits[i].first+range*values[i]);
		}
		fitness.push_back(p_fitness(vars));
	}
	return fitness;
}

Chrom Chrom::bestIndividual(vector<Chrom> Group, double (*p_fitness)(vector<double> vars))
{
	vector<double> fitness = Chrom::calculateFitness(Group, p_fitness);
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
