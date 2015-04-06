#include <vector>
#include <map>
#include <iostream>

#include "alcusi_fcc.h"


using namespace std;
#define R 8.314

class Unary
{
public:
	vector<double> parameter;
	Unary() {};
	Unary(const Unary&);
	void setparameter(vector<double>);
	double getparameter(double T);
};

Unary::Unary(const Unary& unary)
{
	parameter = unary.parameter;
}

void Unary::setparameter(vector<double> p)
{
	parameter = p;
}

double Unary::getparameter(double T)
{
	double temp = 0;
	int i = 0;
	vector<double>::iterator iter;
	for(iter=parameter.begin(); iter<parameter.end(); iter++)
	{
		temp += *iter*pow(T, i);
		i++;
	}
}

class Binary
{
public:
	vector<double> parameter;
	Binary() {};
	Binary(const Binary&);
	void setparameter(vector<double>);
	double getparameter(double x[2], double T);
};

Binary::Binary(const Binary& binary)
{
	parameter = binary.parameter;
}

void Binary::setparameter(vector<double> p)
{
	parameter = p;
}

double Binary::getparameter(double x[2], double T)
{
	double temp = 0;
	int i = 0;
	vector<double>::iterator iter;
	for(iter=parameter.begin(); iter<parameter.end(); iter++)
	{
		temp += *iter*pow(x[0]-x[1], i)*pow(T, i%2);
		i++;
	}
	
}

class Ternary
{
public:
	vector<double> parameter;
	Ternary() {};
	Ternary(const Ternary&);
	double getparameter(double x, double T);
	void setparameter(vector<double>);
};

Ternary::Ternary(const Ternary& ternary)
{
	parameter = ternary.parameter;
}

void Ternary::setparameter(vector<double> p)
{
	parameter = p;
}

double Ternary::getparameter(double x, double T)
{
	double temp = 0;
	int i = 0;
	vector<double>::iterator iter;
	for(iter=parameter.begin(); iter<parameter.end(); iter++)
	{
		temp += *iter*x*pow(T, i%2);
		i++;
	}
}

class Mobility
{
private:
	int cnum;
	int index;
	map<int, map<int, Unary> > unary;
	map<int, map<int, Binary> > binary;
	map<int, map<int, Ternary> > ternary;
	bool flag[3]={0};
public:
	void setUnary(map<int, map<int, Unary> >);
	void setBinary(map<int, map<int, Binary> >);
	void setTernary(map<int, map<int, Ternary> >);
};

void Mobility::setUnary(map<int, map<int, Unary> > unary_)
{
	unary = unary_;
	flag[0] = 1;
}

void Mobility::setBinary(map<int, map<int, Binary> > binary_)
{
	binary = binary_;
	flag[1] = 1;
}

void Mobility::setTernary(map<int, map<int, Ternary> > ternary_)
{
	ternary = ternary_;
	flag [2] = 1;
}



double Mobility_FCC_A1_AL(vector<double> x, double T, vector<double> args={0})
{
	double xa = x[0];
	double xb = x[1];
	double xc = x[2];
	/*
	double mqa_a = mqa_a_1 + mqa_a_2*T;  //a in pure a
	double mqa_b = mqa_b_1 + mqa_b_2*T;  //a in pure b
	double mqa_c = mqa_c_1 + mqa_c_2*T;  //a in pure c
	
	//a in ab alloy
	double mqa_ab = mqa_ab_1 + mqa_ab_2*T + (xa-xb)*(mqa_ab_3 + mqa_ab_4*T) \
		+ (xa-xb)*(xa-xb)*(mqa_ab_5 + mqa_ab_6*T);
	//a in ac alloy
	double mqa_ac = mqa_ac_1 + mqa_ac_2*T + (xa-xb)*(mqa_ac_3 + mqa_ac_4*T) \
		+ (xa-xb)*(xa-xb)*(mqa_ac_5 + mqa_ac_6*T);
	//a in bc alloy 
	double mqa_bc = mqa_bc_1 + mqa_bc_2*T + (xa-xb)*(mqa_bc_3 + mqa_bc_4*T) \
		+ (xa-xb)*(xa-xb)*(mqa_bc_5 + mqa_bc_6*T);  
	//a in abc alloy
	double mqa_abc = mqa_abc_1 + mqa_abc_2*T + xa*(mqa_abc_3 + mqa_abc_4*T) \
		+ xb*(mqa_abc_5 + mqa_abc_6*T)+xc*(mqa_abc_7+mqa_abc_8*T); 
	
	return 1/R/T*exp(-(xa*mqa_a+xb*mqa_b+xc*mqa_c \
		+xa*xb*mqa_ab+xa*xc*mqa_ac+xb*xc*mqa_bc \
		+ xa*xb*xc*mqa_abc)/R/T);
	*/
	return 0;
}

double Mobility_FCC_A1_CU(vector<double> x, double T, vector<double> args={0})
{
	double xa = x[0];
	double xb = x[1];
	double xc = x[2];
	/*
	double mqb_a = -121268.4-89.95*T;  //a in pure a
	double mqb_b = -112499-81.26*T;  //a in pure b
	double mqb_c = -155634.921-81.0257*T;  //a in pure c
	double mqb_ab = 84924.19;;  //a in ab alloy
	double mqb_ac = 1846710.24;  //a in ac alloy
	double mqb_bc = 0; //b in bc alloy
	double mqb_abc = 0; //b in abc alloy
	
	return 1/R/T*exp(-(xa*mqb_a+xb*mqb_b+xc*mqb_c
		+xa*xb*mqb_ab+xa*xc*mqb_ac+cb*xc*mqb_bc
		+xa*xb*xc*mqb_abc)/R/T);
	*/
	return 0;
}
	
double Mobility_FCC_A1_SI(vector<double> x, double T, vector<double> args={0})
{
	double xa = x[0];
	double xb = x[1];
	double xc = x[2];
	/*
	double mqa_a = -123111.6-97.34*T;  //a in pure a
	double mqa_b = -112499-81.26*T;  //a in pure b
	double mqa_c = -155634.921-81.0257*T;  //a in pure c
	double mqa_ab = 0;  //a in ab alloy
	double mqa_ac = 1846710.24;  //a in ac alloy
	
	return 1/R/T*exp(-(xa*mqa_a+xb*mqa_b+xc*mqa_c+xa*xb*mqa_ab+xa*xc*mqa_ac)/R/T);
	*/
	return 0;
}	

typedef double(*funcpointert)(double *, double);
typedef double(*funcpointerk)(vector<double>, double, vector<double>);

class Diffusivity
{
private:
	vector< vector<double> > d;
	vector< vector<funcpointert> > tf;
	vector< funcpointerk > m;
public:
	Diffusivity() {}
	void setsize(int cnum);
	void setfunction(vector< vector<funcpointert> > tf_, vector< funcpointerk > m_);
	vector< vector<double> > getdiffusivitymatrix(vector<double> x, double T, vector<double>);
};

void Diffusivity::setsize(int cnum)
{
	d.resize(cnum);
	vector< vector<double> >::iterator jter;
	for(jter=d.begin(); jter<d.end(); jter++)
	{
		jter->resize(cnum);
	}
	
	tf.resize(cnum);
	
	std::vector< std::vector<funcpointert> >::iterator iter;
	
	for(iter=tf.begin(); iter<tf.end(); iter++)
	{
		iter->resize(cnum);
	}
	
	m.resize(cnum);
}

void Diffusivity::setfunction(vector< vector<funcpointert> > tf_, vector< funcpointerk > m_)
{
	tf = tf_;
	m = m_;
}

vector< vector<double> > Diffusivity::getdiffusivitymatrix(vector<double> x, double T, vector<double> args={})
{
	double arg[3] = {0.1, 0.2, 0.7};
	int cnum = x.size();
	for(int i=0; i<cnum; i++)
	{
		for(int j=0; j<cnum; j++)
		{
			d[i][j] = 0;
			for(int k=0; k<cnum; k++)
			{
				d[i][j] += -x[i]*x[k]*tf[i][j](arg, T)*m[k](x, T, x);
				if(k==j)
				{
					d[i][j] += x[k]*tf[i][j](arg, T)*m[k](x, T, x);
				}
			}
		}
	}
	return d;
}

int main()
{	/*
	vector< vector<funcpointert> > tf = 
	{
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI},
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI},
		{TF_FCC_A1_AL_AL, TF_FCC_A1_AL_CU, TF_FCC_A1_AL_SI}
	};
	vector< funcpointerk > m = {Mobility_FCC_A1_AL, Mobility_FCC_A1_CU, Mobility_FCC_A1_SI};
	vector<double> x = {0.1, 0.2, 0.7};
	
	Diffusivity d;
	d.setsize(3);
	d.setfunction(tf, m);
	vector< vector<double> > dmatrix = d.getdiffusivitymatrix(x, 1000);
	for(int i=0; i<3; i++)
	{
		for(int j=0; j<3; j++)
		{
			cout<<dmatrix[i][j]<<"\t";
		}
		cout<<endl;
	}
	*/
	
	//unary interaction parameter
	vector< vector<double> > ui_al = 
	{{-123111.6, -97.34},
	{-112499, -81.26},
	{-155634.921, -81.0257}};
	
	vector< vector<double> > ui_mg = 
	{{-121268.4, -89.95},
	{-112499, -81.26},
	{-155634.921, -81.0257}};
	
	vector< vector<double> > ui_si =
	{{-117600, -93.046},
	{-112499, -81.26},
	{-155634.921, -81.0257}};
	
	//binary interaction parameter
	vector< vector<double> > bi_al =
	{{1846710.24}};
	
	vector< vector<double> > bi_mg = 
	{{84924.19}};
	
	vector< vector<double> > bi_si =
	{{-152613.876}};
	
	//ternary interaction parameter
	vector< vector<double> > ti_al =
	{{-1.20434323E+05}};
	
	vector< vector<double> > ti_mg = 
	{{9.97320005E+03}};
	
	vector< vector<double> > ti_si = 
	{{9.97689484E+03}};
	return 0;
}
