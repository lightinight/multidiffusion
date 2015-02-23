/*
#include <vector>
#include <map>
#include <cmath>
#include <iostream>
using namespace std;
*/
#define R 8.314
#include "alcusi_fcc.h"

class Polynomial
{
private:
    vector<double> series;
    //const const*T const*T*ln(T) const/T const*T**2
public:
    Polynomial();
    Polynomial(vector<double>);
    void setseries(vector<double>);
    double evaluate(double);
    double& getpointer(int);
    void display();
};

Polynomial::Polynomial()
{
    series.resize(5);
}

Polynomial::Polynomial(vector<double> series_)
{
    series.resize(5);
    setseries(series_);
}

void Polynomial::setseries(vector<double> series_)
{
    vector<double>::iterator iter;
    int i = 0;
    for(iter=series_.begin(); iter<series_.end() && i<5; iter++, i++)
    {
        series[i] = *iter;
    }
}

double Polynomial::evaluate(double var)
{
    return series[0] + series[1]*var + series[2]*var*log(var)\
        + series[3]/var + series[4]*var*var;
}

double& Polynomial::getpointer(int index)
{
    return series[index];
}

void Polynomial::display()
{
    vector<double>::iterator iter;
    for(iter=series.begin(); iter<series.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
}

class Parameter
{
public:
    Polynomial poly;
    int rank;
    int elementindex;
    vector<int> constituent;
public:
    Parameter() {;}
    Parameter(int, vector<int>, int, vector<double>);
    void setparameter(int, vector<int>, int, vector<double>);
    double evaluate(vector<double> x, double var);
    void display();
};

Parameter::Parameter(int index, vector<int> constituent_, int rank_, vector<double> series_)
{
    setparameter(index, constituent_, rank_, series_);
}

void Parameter::setparameter(int index, vector<int> constituent_, int rank_, vector<double> series_)
{
    poly.setseries(series_);
    rank = rank_;
    elementindex = index;
    constituent = constituent_;
}

double Parameter::evaluate(vector<double> x, double var)
{
    int type = constituent.size();
    if(type==1) return x[constituent[0]]*poly.evaluate(var);
    else if(type==2) return x[constituent[0]]*x[constituent[1]]* \
                            pow(x[constituent[0]]-x[constituent[1]], rank)*poly.evaluate(var);
    else if(type==3) return x[constituent[0]]*x[constituent[1]]* x[constituent[2]]*x[rank]*poly.evaluate(var);
    else return 0;
}

void Parameter::display()
{
    cout<<elementindex<<": ";
    vector<int>::iterator iter;
    for(iter=constituent.begin(); iter<constituent.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<", "<<rank<<endl;
    poly.display();
}

class Mobility
{
protected:
    map<int, vector<Parameter> > db;
public:
    Mobility() {;}
    Mobility(const Mobility&);
    void setmobility(const Mobility&);
    void additem(int, vector<Parameter>);
    double evaluate(int, vector<double> x, double T);
    void display();
};

Mobility::Mobility(const Mobility& m)
{
    db = m.db;
}

void Mobility::setmobility(const Mobility& m)
{
    db = m.db;
}

void Mobility::additem(int index,  vector<Parameter> paras)
{
    db.insert(map<int, vector<Parameter> >::value_type(index, paras));
}

double Mobility::evaluate(int index, vector<double> x, double T)
{
   vector<Parameter>::iterator iter;
   double temp;
   for(iter=db[index].begin(); iter<db[index].end(); iter++)
   {
       temp += iter->evaluate(x, T);
   }
   return exp(temp/R/T)/R/T;
}

void Mobility::display()
{
    map<int, vector<Parameter> >::iterator iter;
    for(iter=db.begin(); iter != db.end(); iter++)
    {
        vector<Parameter>::iterator jter;
        for(jter=iter->second.begin(); jter<iter->second.end(); jter++)
        {
            jter->display();
        }
    }
}

class Variable
{
public:
    int elementindex;
    int rank;
    vector<int> constituent;
    int seriesindex;
    Variable() {;}
    Variable(int, vector<int>, int, int);
    void setvariable(int, vector<int>, int, int);
};

Variable::Variable(int index, vector<int> constituent_, int rank_, int seriesindex_)
{
    setvariable(index, constituent_, rank_, seriesindex_);
}

void Variable::setvariable(int index, vector<int> constituent_, int rank_, int seriesindex_)
{
    elementindex = index;
    constituent = constituent_;
    rank = rank_;
    seriesindex = seriesindex_;
}


//element parameter_index series_index
class ModifiedMobility:public Mobility
{
public:
    ModifiedMobility(){;}
    double& getpointer(Variable);
};

double& ModifiedMobility::getpointer(Variable var)
{
   vector<Parameter>::iterator iter;
   for(iter=db[var.elementindex].begin(); iter<db[var.elementindex].end(); iter++)
   {
       bool flag = 1;
       vector<int>::iterator jter, kter;
       jter = var.constituent.begin();
       kter = iter->constituent.begin();
       for(; jter<var.constituent.end() && *jter == *kter && var.rank==iter->rank; jter++, kter++){;}
       if(jter==var.constituent.end())
       {
           return iter->poly.getpointer(var.seriesindex);
           break;
       }
   } 
}

typedef double(*funcpointert)(vector<double>, double);
typedef double(*funcpointerk)(vector<double>, double, vector<double>);

class Diffusivity
{
private:
	vector< vector<double> > di;
    vector< vector<double> > dc;
	vector< vector<funcpointert> > tf;
	ModifiedMobility m;
public:
	Diffusivity() {;}
    Diffusivity(const Diffusivity&);
	void setsize(int cnum);
	void setthermofactor(vector< vector<funcpointert> > tf_);
    void setmobility(ModifiedMobility);
	vector< vector<double> > getintrinsicmatrix(vector<double> x, double T, vector<double>);
    vector< vector<double> > getchemicalmatrix(vector<double> x, double T, int ref);
};

Diffusivity::Diffusivity(const Diffusivity& temp)
{
    di = temp.di;
    dc = temp.dc;
    tf = temp.tf;
    m = temp.m;
}

void Diffusivity::setsize(int cnum)
{
	di.resize(cnum);
	vector< vector<double> >::iterator jter;
	for(jter=di.begin(); jter<di.end(); jter++)
	{
		jter->resize(cnum);
	}
    
    dc.resize(cnum-1);
	vector< vector<double> >::iterator kter;
	for(kter=dc.begin(); kter<dc.end(); kter++)
	{
		kter->resize(cnum-1);
	}
	
	tf.resize(cnum);
	
	std::vector< std::vector<funcpointert> >::iterator iter;
	
	for(iter=tf.begin(); iter<tf.end(); iter++)
	{
		iter->resize(cnum);
	}
}

void Diffusivity::setthermofactor(vector< vector<funcpointert> > tf_)
{
	tf = tf_;
}

void Diffusivity::setmobility(ModifiedMobility m_)
{
    m.setmobility(m_);
}

vector< vector<double> > Diffusivity::getintrinsicmatrix(vector<double> x, double T, vector<double> args={})
{
	int cnum = di.size();
	for(int i=0; i<cnum; i++)
	{
		for(int j=0; j<cnum; j++)
		{
			di[i][j] = 0;
			for(int k=0; k<cnum; k++)
			{
				di[i][j] += -x[i]*x[k]*tf[i][j](x, T)*m.evaluate(k, x, T);
				if(k==j)
				{
					di[i][j] += x[k]*tf[i][j](x, T)*m.evaluate(k, x, T);
				}
			}
		}
	}
	return di;
}

vector< vector<double> > Diffusivity::getchemicalmatrix(vector<double> x, double T, int ref=-1)
{
    getintrinsicmatrix(x, T);
    int cnum = di.size();
    if(ref==-1) ref = cnum-1;
    for(int i=0; i<cnum-1; i++)
    {
        for(int j=0; j<cnum-1; j++)
        {
            dc[i][j] = di[i][j]-di[i][ref];
        }
    }
    return dc;
}

//an example for using the Mobility class
ModifiedMobility makemobility()
{	
    //specify parameters for different interaction parameters.
    //the parameters for a Parameter means: 
    //    element-index, constituent, rank, polynomial-coefficient
    vector< Parameter > al = {
        Parameter(0, {0},       0, {-123111.6, -97.37}),
        //Parameter(0, {1},       0, {-112499, -81.26}),
        //Parameter(0, {2},       0, {-155634.921, -81.0257}),
        //Parameter(0, {0, 2},    0, {1846710.24}),
        //Parameter(0, {0, 1, 2}, 0, {9.97320005e3})
    };
    vector< Parameter > mg = {
        Parameter(1, {0},       0, {-121268.4, -89.95}),
        //Parameter(1, {1},       0, {-112499, -81.26}),
        //Parameter(1, {2},       0, {-155634.921, -81.0257}),
        //Parameter(1, {0, 1},    0, {84924.19}),
        //Parameter(1, {0, 1, 2}, 0, {-1.20434323E5})
    };
    vector< Parameter > si = {
        Parameter(2, {0},       0, {-117600, -93.046}),
        //Parameter(2, {1},       0, {-112499, -81.26}),
        //Parameter(2, {2},       0, {-155634.921, -81.0257}),
        //Parameter(2, {0, 2},    0, {-152613.876}),
        //Parameter(2, {0, 1, 2}, 0, {9.97320005e3})
    };
    //define a modified-mobility object
    ModifiedMobility almgsi;
    //add the kinetic-data for different elements
    almgsi.additem(0, al);
    almgsi.additem(1, mg);
    almgsi.additem(2, si);
    //an example that shows how to evaluate the mobility of a element
    almgsi.evaluate(0, {0.1, 0.2, 0.7}, 900);
    //here are a example of how to implement a changeable parameter
    //the parameters for a Variable means:
    //    element-index constituent rank the-index-of-poly-series
    //Variable var1(0, {0, 1, 2}, 0, 1);
    //first: get the pointer to a element of poly-series of a parameter
    //double& temp = almgsi.getpointer(var1);
    //almgsi.display();
    //assign a new value to the pointer
    //almgsi.display();
    ModifiedMobility t(almgsi);
    t.display();
	return t;
}

//an example shows how to use the diffusivity class
Diffusivity makediffusivity()
{
    //define a Diffusivity object
    Diffusivity d;
    //allocate memory-storage for a Diffusivity obejct
	d.setsize(3);
    
    //define the thermodynamic factor
    vector< vector<funcpointert> > tf = 
	{
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI},
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI},
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI}
	};
    //assign thermodynamic factors to the Diffusivity obejct 
	d.setthermofactor(tf);
    //assign value to the mobility 
    d.setmobility(makemobility());
    //an example for evaluating the Diffusivity-Matrix
    vector<double> x = {0.8, 0.1, 0.1};
    vector< vector<double> > dmatrix = d.getintrinsicmatrix(x, 1000);
	//print it out
    for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			cout<<dmatrix[i][j]<<"\t";
		}
		cout<<endl;
	}
    return d;
}

int main1()
{
    //example2();
    return 0;
}