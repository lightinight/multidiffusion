#include <iostream>
#include <vector>
#include "Splines.hh"
using namespace SplinesLoad ;
using namespace std;
int main()
{
CubicSpline spline ;
vector<double> x = {1,2,3,4} ;
vector<double> y = {1,4,9,16} ;
spline.build(x,y) ; // build a cubic spline with 4 points

cout << spline(1.2) << '\n';     // spline at x = 1.1
cout << spline.D(1.1) << '\n';   // spline first derivative at x = 1.1
cout << spline.DD(1.1) << '\n';  // spline second derivative at x = 1.1
cout << spline.DDD(1.1) << '\n'; // spline third derivative at x = 1.1
}
