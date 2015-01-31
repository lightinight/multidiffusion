#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class MultiDiffusion
{
private:
	vector< vector<double> > conc;
	vector< vector<double> > dconc;
public:
	int cnum;
	int gridnum;
	MultiDiffusion(){};
	void initializeVariable(vector< vector<double> >* var, int cnum_, int gridnum_);
	void setVariable(vector< vector<double> >* var, double value);
	void setSystem(int, int);
	void setCouple(vector<int>, vector<double>, vector<double>);
	void calculateIncrement();
	void update();
	void outputVariable(vector< vector<double> > var, string pathname);
	void outputAll(string pathname);
	void evolution(int);
};

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
	vector< vector<double> > D = {{1.0e-8, 1.0e-8}, {1.0e-8, 1.0e-8}};

	for(int i=0; i<cnum-1; i++)
	{
		for(int j=1; j<gridnum-1; j++)
		{
			dconc[i][j] = 0;
			for(int k=0; k<cnum-1; k++)
			{
				dconc[i][j] += D[i][k]*(conc[k][j+1]-conc[k][j])+
					D[i][k]*(conc[k][j]-conc[k][j-1]);
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

int main()
{
	int cnum = 3;
	int gridnum = 500;
	vector<int> pos = {249, 249, 249};
	vector<double> cleft = {0.2, 0.3, 0.5};
	vector<double> cright = {0.6, 0.2, 0.2};
	MultiDiffusion temp;
	temp.setSystem(cnum, gridnum);
	temp.setCouple(pos, cleft, cright);
	string pathname = "test.txt";
	//temp.outputAll(pathname);
	temp.evolution(10000);
	temp.outputAll(pathname);
}

