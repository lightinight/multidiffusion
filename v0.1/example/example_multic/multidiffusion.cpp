#include "multidiffusion.h"

void MultiDiffusion::initializeVariable(vector< vector<double> >* var, int cnum_, int gridnum_)
{
	cnum = cnum_;
	gridnum = gridnum_;
	cout<<cnum<<" "<<gridnum<<endl;
	var->resize(cnum);
	vector< vector<double> >::iterator iter;
	for(iter=var->begin(); iter<var->end(); iter++)
	{
		iter->resize(gridnum);
	}
	setVariable(var, 0.0);
}
void MultiDiffusion::setVariable(vector< vector<double> >* var, double value)
{
	vector< vector<double> >::iterator iter;
	for(iter=var->begin(); iter<var->end(); iter++)
	{
		vector<double>::iterator jter;
		for(jter=iter->begin(); jter<iter->end(); jter++)
		{
			*jter = value;
		}
	}
}
void MultiDiffusion::setSystem(int cnum_, int gridnum_)
{
	initializeVariable(&conc, cnum_, gridnum_);
	initializeVariable(&dconc, cnum_, gridnum_);
}
void MultiDiffusion::setCouple(vector<int> pos, vector<double> cleft, vector<double> cright)
{
	for(int i=0; i<cnum; i++)
	{
		for(int j=0; j<gridnum; j++)
		{
			if(j<=pos[i])
			{
				conc[i][j] = cleft[i];
			}
			else
			{
				conc[i][j] = cright[i];
			}
		}
	}
}
void MultiDiffusion::outputVariable(vector< vector<double> > var, string pathname)
{
	stringstream outbufer;
	ofstream outfile;
	outfile.open(pathname.c_str(), ios::out);
	for(int i=0; i<gridnum; i++)
	{
		for(int j=0; j<cnum; j++)
		{
			outbufer<<var[j][i]<<"\t";
		}
		outbufer<<endl;
	}
	outfile<<outbufer.rdbuf();
	outfile.close();
}
void MultiDiffusion::outputAll(string pathname)
{
	outputVariable(conc, pathname);
}
void MultiDiffusion::calculateIncrement()
{
    double T = 900;
    vector<double> x;
    double d = 1.0e-10;
    x.resize(cnum);
	for(int j=1; j<gridnum-1; j++)
	{
        for(int k=0; k<cnum; k++) {x[k] = conc[k][j];}
		for(int i=0; i<cnum-1; i++)
		{
			dconc[i][j] = 0;
			for(int k=0; k<cnum-1; k++)
			{
				dconc[i][j] += d*(conc[k][j+1]-conc[k][j])+
					d*(conc[k][j]-conc[k][j-1]);
			}
		}
	}
}
void MultiDiffusion::update()
{
	double dx = 1.0e-5;
	double dt = 1.0e-6;
	for(int i=0; i<cnum-1; i++)
	{
		for(int j=1; j<gridnum-1; j++)
		{
			conc[i][j] += dt/dx/dx*dconc[i][j];
		}
	}
	
	for(int j=1; j<gridnum-1; j++)
	{
		double temp = 0;
		for(int k=0; k<cnum-1; k++)
		{
			temp += conc[k][j];
		}
		conc[cnum-1][j] = 1-temp;
	}
}
void MultiDiffusion::evolution(int time)
{
	for(int tstep=0; tstep<time; tstep++)
	{
		calculateIncrement();
		update();
		cout<<tstep<<endl;
	}
}
