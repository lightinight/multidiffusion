#include "multidiffusion.h"
void MultiDiffusion::setDiffusivity(const Diffusivity& temp)
{
    d = temp;
}
void MultiDiffusion::__initVariable(vector< vector<double> >* var, int _cnum, int _gridnum)
{
	cnum = _cnum;
	gridnum = _gridnum;
	cout<<_cnum<<" "<<_gridnum<<endl;
	var->resize(cnum);
	vector< vector<double> >::iterator iter;
	for(iter=var->begin(); iter<var->end(); iter++)
	{
		iter->resize(gridnum);
	}
	__setVariable(var, 0.0);
}
void MultiDiffusion::__setVariable(vector< vector<double> >* var, double value)
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
void MultiDiffusion::setSystem(int _cnum, int _gridnum)
{
	__initVariable(&conc, _cnum, _gridnum);
	__initVariable(&dconc, _cnum, _gridnum);
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
void MultiDiffusion::calculateIncrement(double T)
{
    vector<double> x;
    x.resize(cnum);
	for(int j=1; j<gridnum-1; j++)
	{
        for(int k=0; k<cnum; k++) {x[k] = conc[k][j];}
        vector< vector<double> > D = d.getchemicalmatrix(x, T);
		for(int i=0; i<cnum-1; i++)
		{
			dconc[i][j] = 0;
			for(int k=0; k<cnum-1; k++)
			{
				dconc[i][j] += D[i][k]*(conc[k][j+1]-conc[k][j])-
					D[i][k]*(conc[k][j]-conc[k][j-1]);
			}
		}
	}
}
void MultiDiffusion::update(double dx, double dt)
{
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
		conc[cnum-1][j] = 1.0-temp;
	}
}
void MultiDiffusion::evolution(int time, double T, double dx, double dt)
{
    checkConserved();
	for(int tstep=0; tstep<time; tstep++)
	{
		calculateIncrement(T);
		update(dx, dt);
		cout<<tstep<<endl;		
	}
	checkConserved();
}
void MultiDiffusion::checkConserved()
{
    for(int i=0; i<cnum; i++)
    {
        double sum = 0;
        for(int j=0; j<gridnum; j++)
        {
            sum += conc[i][j];
        }
        cout<<"concentration "<<i<<": "<<sum/gridnum<<endl;
    }
}
