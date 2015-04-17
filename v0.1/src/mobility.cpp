#include "mobility.h"
using namespace std;
Variable::Variable(int index, vector<int> constituent_, int rank_, int seriesindex_)
{
    setvariable(index, constituent_, rank_, seriesindex_);
}
void Variable::setvariable(int index, vector<int> constituent_, int rank_, int seriesindex_)
{
    elementindex = index;
    constituent = constituent_;
     parameter_order = rank_;
    seriesindex = seriesindex_;
}
//element parameter_index series_index
//Mobility database

Mobility::Mobility(const Mobility& m)
{
    //initialize the Mobility with another Mobility object.
    db = m.db;
}
void Mobility::setmobility(const Mobility& m)
{
    //copy the database from another Mobility object.
    db = m.db;
}
void Mobility::additem(int index,  vector<Parameter> paras)
{
    //add a parameter to the present database.
    db.insert(map<int, vector<Parameter> >::value_type(index, paras));
}
double Mobility::evaluate(int index, vector<double> x, double T)
{
   //get the mobility of a certain element with index
   //by using the composition and temperature of the system.
   vector<Parameter>::iterator iter;
   double temp = 0;
   for(iter=db[index].begin(); iter<db[index].end(); iter++)
   {
       temp += iter->evaluate(x, T);
   }
   return exp(temp/8.314/T)/8.314/T;
}
void Mobility::display()
{
    //display the mobility database on the screen.
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
double& Mobility::getpointer(Variable var)
{
   vector<Parameter>::iterator iter;
   for(iter=db[var.elementindex].begin(); iter<db[var.elementindex].end(); iter++)
   {
       bool flag = 1;
       vector<int>::iterator jter, kter;
       jter = var.constituent.begin();
       kter = iter->constituent.begin();
       //verify whether two pointer are equal.
       for(; jter<var.constituent.end() \
           && *jter == *kter && \
           var.parameter_order==iter->parameter_order; \
           jter++, kter++){;}
       if(jter==var.constituent.end())
       {
           return iter->poly.getpointer(var.seriesindex);
           break;
       }
   } 
}
Diffusivity::Diffusivity(const Diffusivity& temp)
{
    //copy a Diffusivity object.
    di = temp.di;
    dc = temp.dc;
    tf = temp.tf;
    m = temp.m;
}
void Diffusivity::set_comp_num(int cnum)
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
	/*
	tf.resize(cnum);
	
	std::vector< std::vector<funcpointert> >::iterator iter;
	
	for(iter=tf.begin(); iter<tf.end(); iter++)
	{
		iter->resize(cnum);
	}*/
}
void Diffusivity::setthermofactor( \
    vector< vector<funcpointert> > tf_
    )
{
	tf = tf_;
}
void Diffusivity::setmobility(Mobility m_)
{
    m.setmobility(m_);
}
vector< vector<double> > \
    Diffusivity::getintrinsicmatrix( \
        vector<double> x, double T, vector<double> args={} \
    )
{
	int cnum = di.size();
	for(int i=0; i<cnum; i++)
	{
		for(int j=0; j<cnum; j++)
		{
			di[i][j] = 0;
			for(int k=0; k<cnum; k++)
			{
			    double delta = k==i?1.0:0.0;
				di[i][j] += (delta-x[i])*x[k]*tf[k][j](x, T)*m.evaluate(k, x, T);
			}
		}
	}
	return di;
}
vector< vector<double> > \
    Diffusivity::getchemicalmatrix( \
        vector<double> x, double T, int ref
    )
{
    getintrinsicmatrix(x, T);
    int cnum = di.size();
    if(ref==-1) ref = cnum-1;
    int a=0;
    for(int i=0; i<cnum-1; i++, a++)
    {
        if(i==ref)
            a++;
        int b=0;
        for(int j=0; j<cnum-1; j++, b++)
        {
            if(j==ref)
                b++;
            dc[i][j] = di[a][b]-di[a][ref];
        }
    }
    return dc;
}

double& Diffusivity::getpointer(Variable var)
{
    return m.getpointer(var);
}
