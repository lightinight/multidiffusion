#include <iostream>
#include <vector>
#include <cstdlib>
#include <time.h> 
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm> 
#include <string.h>
using namespace std;


int traceLU(vector<double> AIH, vector<double> AII, vector<double> AIJ, vector<double> f)
{
	int N = AII.size();
	/*
	srand((unsigned)time(NULL));
	vector<double> AIH, AII, AIJ, f;
	for(int i=0; i<N; i++)
	{
		AIH.push_back(rand()%47+1);
		AII.push_back(rand()%47+1);
		AIJ.push_back(rand()%47+1);
		f.push_back(rand()%47);
	}
	*/
	vector<double> beta(AII);
	beta[0] = AIJ[0]/AII[0];
	for(int j=1; j<N; j++)
	{
		beta[j] = AIJ[j]/(AII[j]-AIH[j]*beta[j-1]);
	}
	
	vector<double> y;
	y.push_back(f[0]/AII[0]);
	for(int k=1; k<N; k++)
	{
		y.push_back((f[k]-AIH[k]*y[k-1])/(AII[k]-AIH[k]*beta[k-1]));
	}
	
	vector<double> x(AII);
	x[N-1] = y[N-1];
	for(int l=N-2; l>=0; l--)
	{
		x[l] = y[l]-beta[l]*x[l+1];
	}
	
	for(int m=0; m<N; m++)
	{
		cout<<x[m]<<endl;
	}
	return 0;	
}

int main()
{
	/*
	double aih[2] = {0, 1.2};
	double aii[2] = {2, 4.7};
	double aij[2] = {2.3, 0};
	double f[2] = {2.1, 3.7};
	vector<double> AIH(aih[0], aih[1]);
	vector<double> AII(aii[0], aii[1]);
	vector<double> AIJ(aij[0], aij[1]);
	vector<double> F(f[0], f[1]);
	*/
	vector<double> AIH{0, 1.3, 2.4};
	vector<double> AII{2, 4.7, 2.7};
	vector<double> AIJ{3.5, 1.8, 2.7};
	vector<double> F{2.1, 3.7, 1.7};
	Solve(AIH, AII, AIJ, F);
}
