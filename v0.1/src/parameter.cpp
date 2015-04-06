#include "parameter.h"
Polynomial::Polynomial()
{
    series.resize(5);
}
Polynomial::Polynomial(vector<double> _series)
{
    series.resize(5);
    setseries(_series);
}
void Polynomial::setseries(vector<double> _series)
{
    vector<double>::iterator iter;
    int i = 0;
    for(iter=_series.begin(); \
        iter<_series.end() && i<5; iter++, i++)
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
double& Polynomial::getpointer(int _index)
{
    return series[_index];
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
void Polynomial::test()
{
    double v1[] = {10.0};
    double v2[] = {10.0, 20.0};
    double v3[] = {10.0, 20.0, 30.0, 40.0};
    vector<double> vv1(v1, v1+1);
    vector<double> vv2(v2, v2+2);
    vector<double> vv3(v3, v3+4);
    Polynomial p1(vv1);
    Polynomial p2(vv2);
    Polynomial p3(vv3);
    double r1[] = {p1.evaluate(10.0), 
                   p2.evaluate(10.0),
                   p3.evaluate(10.0)};
    double r2[] = {10.0, 210.0, 904.776};
    for(int i=0; i<3; ++i)
    {
        if(abs(r1[i]-r2[i]) < 1.0e-3)
        {
            cout<<".";
        }
        else
        {
            cout<<"F";
        }
    }
    cout<<endl;
}
Parameter::Parameter(int _index, vector<int> _constituent, int _order, vector<double> _series)
{
    setparameter(_index, _constituent, _order, _series);
}
void Parameter::setparameter(int _index, vector<int> _constituent, int _order, vector<double> _series)
{
    poly.setseries(_series);
    parameter_order = _order;
    index = _index;
    constituent = _constituent;
}
double Parameter::evaluate(vector<double> x, double var)
{
    int type = constituent.size();
    if(type==1) return x[constituent[0]]*poly.evaluate(var);
    else if(type==2)
        return x[constituent[0]]*x[constituent[1]]* \
                pow((x[constituent[0]]-x[constituent[1]]), parameter_order) * \
                poly.evaluate(var);
    else if(type==3) 
        return x[constituent[0]]*x[constituent[1]]* \
                x[constituent[2]]*x[parameter_order]*poly.evaluate(var);
    else return 0;
}
void Parameter::display()
{
    cout<<index<<": ";
    vector<int>::iterator iter;
    for(iter=constituent.begin(); \
        iter<constituent.end(); iter++)
    {
        cout<<*iter<<" ";
    }
    cout<<", "<<parameter_order<<endl;
    poly.display();
}
