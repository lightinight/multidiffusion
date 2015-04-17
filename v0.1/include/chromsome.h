#ifndef _DEFS_H
#include "defs.h"
#endif

#ifndef _BINARY_H
#include "binary.h"
#endif

#define _CHROMSOME_H

class Chrom
{
public:
    //data member
    int chrmnum;
	std::vector< binary > chrm;
	std::vector< std::pair<double, double> > limits;
	std::vector< int > bitslist;
	
	//function member
	void setChrmNum(int);
	void setBitsList(std::vector<int>);
	void setLimits(std::vector< std::pair<double, double> >);
	void showLimits();
	std::vector< double > getValue();
	int getChrmNum() {return chrmnum;}
	void randomConstructChrm();
	std::vector< Chrom > crossOverAB(Chrom chrm1, Chrom chrm2);
	std::vector< Chrom > crossOverGroup(std::vector< Chrom > Group);
	Chrom mutateA(Chrom chrms, int mutatetimes);
	std::vector< Chrom > mutateGroup(std::vector< Chrom > Group, double Mutaterate, int);
	void show();
	
	std::vector< Chrom > selection(std::vector<Chrom> chrms, double (*p_fitness)(std::vector<double>), int selectednum);
	std::vector<Chrom> newGroup(int groupnum);
	Chrom singleObjectEvolution(int times, int groupnum, double (*p_fitness)(std::vector<double> vars));
	std::vector<double> calculateFitness(std::vector<Chrom> chrms, double (*p_fitness)(std::vector<double> vars));
	Chrom bestIndividual(std::vector<Chrom> Group, double (*p_fitness)(std::vector<double> vars));

};
