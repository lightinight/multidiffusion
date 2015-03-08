/*
__all__ = ["Polynomial", "Parameter"]
*/
class Polynomial
{
private:
    vector<double> series;
    //const + const*T + const*T*ln(T) + const/T + const*T**2
    //v1 v2 v3 v4
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
    for(iter=series_.begin(); \
        iter<series_.end() && i<5; iter++, i++)
    {
        series[i] = *iter;
    }
}
double Polynomial::evaluate(double var)
{
    return series[0] + series[1]*var + \
        series[2]*var*log(var) + \
        series[3]/var + series[4]*var*var;
}
double& Polynomial::getpointer(int index)
{
    return series[index];
}
void Polynomial::display()
{
    vector<double>::iterator iter;
    for(iter=series.begin(); \
        iter<series.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<endl;
}
class Parameter
{
public:
    Polynomial poly;
    int parameter_order;
    int elementindex;
    vector<int> constituent;
public:
    Parameter() {;}
    Parameter(int, vector<int>, int, vector<double>);
    void setparameter(int, vector<int>, int, vector<double>);
    double evaluate(vector<double> x, double var);
    void display();
};
Parameter::Parameter(int index, \
    vector<int> constituent_, \
    int rank_, vector<double> series_
    )
{
    setparameter(index, constituent_, rank_, series_);
}
void Parameter::setparameter(int index, \
    vector<int> constituent_, int rank_, \
    vector<double> series_
    )
{
    poly.setseries(series_);
    parameter_order = rank_;
    elementindex = index;
    constituent = constituent_;
}
double Parameter::evaluate(vector<double> x, double var)
{
    int type = constituent.size();
    if(type==1) return x[constituent[0]]*poly.evaluate(var);
    else if(type==2) 
        return x[constituent[0]]*x[constituent[1]]* \
                pow(x[constituent[0]]-x[constituent[1]], parameter_order) * \
                poly.evaluate(var);
    else if(type==3) 
        return x[constituent[0]]*x[constituent[1]]* \
                x[constituent[2]]*x[parameter_order]*poly.evaluate(var);
    else return 0;
}
void Parameter::display()
{
    cout<<elementindex<<": ";
    vector<int>::iterator iter;
    for(iter=constituent.begin(); \
        iter<constituent.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<", "<<parameter_order<<endl;
    poly.display();
}

/*
this part implement some basic tools for calculating the diffusivity.
__all__ = ["Mobility": a kinetic database,
           "Diffusivity": a diffusivity database,
           "Variable": 
               a variable that helps with modifing the Mobility]
*/
class Variable
{
    //this is a class using for modifing the object of 
    //Mobility class.
public:
    int elementindex;
    int  parameter_order;
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
     parameter_order = rank_;
    seriesindex = seriesindex_;
}
//Mobility database
class Mobility
{
    //this is a kinetic database.
protected:
    map<int, vector<Parameter> > db;
public:
    Mobility() {;}
    Mobility(const Mobility&);
    void setmobility(const Mobility&);
    void additem(int, vector<Parameter>);
    double evaluate(int, vector<double> x, double T);
    void display();
    double& getpointer(Variable);
};
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
   double temp;
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
//define the pointer-type for the functions of 
//mobility and thermodynamic factor.
typedef double(*funcpointert)(vector<double>, double);
typedef double(*funcpointerk)(vector<double>, double, vector<double>);
class Diffusivity
{
    //a database of mobility and thermodynamic factor.
private:
	vector< vector<double> > di;
    vector< vector<double> > dc;
	vector< vector<funcpointert> > tf;
	Mobility m;
public:
	Diffusivity() {;}
    Diffusivity(const Diffusivity&);
	void set_comp_num(int cnum);
	void setthermofactor(vector< vector<funcpointert> > tf_);
    void setmobility(Mobility);
	vector< vector<double> > getintrinsicmatrix(vector<double> x, double T, vector<double>);
    vector< vector<double> > getchemicalmatrix(vector<double> x, double T, int ref);
};
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
	
	tf.resize(cnum);
	
	std::vector< std::vector<funcpointert> >::iterator iter;
	
	for(iter=tf.begin(); iter<tf.end(); iter++)
	{
		iter->resize(cnum);
	}
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
vector< vector<double> > \
    Diffusivity::getchemicalmatrix( \
        vector<double> x, double T, int ref=-1
    )
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
/*
this part accounts for solving the diffusion 
equations for multi-components system.
*/
class MultiDiffusion
{
private:
	vector< vector<double> > conc;
	vector< vector<double> > dconc;
    Diffusivity d;
public:
	int cnum;  //the number of component.
	int gridnum;  //the number of grid ffor finite differential method.
	double xstep;  //the space step.
	double tstep;  //the time space.
	double temperature;  //the temperature of the system.
	double time;  //the time of simulation.
	MultiDiffusion(){};
    void setDiffusivity(const Diffusivity&);
	void initializeVariable(vector< vector<double> >* var, int cnum_, int gridnum_);
	void setVariable(vector< vector<double> >* var, double value);
	void setSimCond(int, int, double, double, double, double);
	void setCouple(vector<int>, vector<double>, vector<double>);
	void calculateIncrement();
	void update();
	void outputVariable(vector< vector<double> > var, string pathname);
	void outputAll(string pathname);
	void evolution(int);
	void evolution();
};
void MultiDiffusion::setDiffusivity(const Diffusivity& temp)
{
    d = temp;
}
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
void MultiDiffusion::setSimCond(int cnum_, int gridnum_, double dx_, double dt_, double temperature_, double time_)
{
	initializeVariable(&conc, cnum_, gridnum_);
	initializeVariable(&dconc, cnum_, gridnum_);
	cnum = cnum_;
	gridnum = gridnum_;
	tstep = dt_;
	xstep = dx_;
	temperature = temperature_;
	time = time_;
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
    vector<double> x;
    x.resize(cnum);
	for(int j=1; j<gridnum-1; j++)
	{
        for(int k=0; k<cnum; k++) {x[k] = conc[k][j];}
        vector< vector<double> > D = d.getchemicalmatrix(x, temperature);
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
void MultiDiffusion::update()
{
	for(int i=0; i<cnum-1; i++)
	{
		for(int j=1; j<gridnum-1; j++)
		{
			conc[i][j] += tstep/xstep/xstep*dconc[i][j];
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
void MultiDiffusion::evolution(int times)
{
	for(int tstep=0; tstep<time; tstep++)
	{
		calculateIncrement();
		update();
		cout<<tstep<<endl;
	}
}
void MultiDiffusion::evolution()
{
    int times = time/tstep;
	for(int tstep=0; tstep<times; tstep++)
	{
		calculateIncrement();
		update();
		cout<<tstep<<endl;
	}
}
