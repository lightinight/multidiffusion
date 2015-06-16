#include "couple.h"
using namespace std;
void Couple::presetting()
{
    is_getinitpos = 0;
    is_interpolated = 0;
    dx = -1;
}

Couple::Couple()
{
    presetting();
}

Couple::Couple(string _couplename, vector<Profile> profiles)
{
    presetting();
    couplename = _couplename;
    vector<Profile>::iterator iter;
    for(iter=profiles.begin(); iter<profiles.end(); ++iter)
    {
        pair<string, Profile> pair1 = \
            make_pair(iter->getElementName(), *iter);
        couple.insert(pair1);
    }
}

Couple::Couple(string _couplename)
{
    presetting();
    couplename = _couplename;
}

void Couple::insert(vector<Profile> profiles)
{
    vector<Profile>::iterator iter;
    for(iter=profiles.begin(); iter<profiles.end(); ++iter)
    {
        elementnames.push_back(iter->getElementName());
        //iter->sort();
        //iter->smooth(3);
        pair<string, Profile> pair1 = \
            make_pair(iter->getElementName(), *iter);
        couple.insert(pair1);
    }
    getInitPos();
}

void Couple::setName(string name)
{
    couplename = name;
}

vector< string > Couple::getElementsName()
{
    return elementnames;
}

void Couple::show()
{
    map<string, Profile>::iterator iter;
    cout<<"\ncouplename: "<<couplename<<endl;
    for(iter=couple.begin(); iter!=couple.end(); ++iter)
    {
        iter->second.show();
    }
}

void Couple::makeConserved(string ElementName)
{
    if(ElementName.compare(0, ElementName.size(), "NONE") == 0)
    {
        return;
    }
    if(!is_interpolated) makeInterpolated();
    //int N = (posright - posleft)/dx;

}

double Couple::getInitPos()
{
    if(is_getinitpos) return initpos;

    map<string, Profile>::iterator iter;
    double left = 0;
    double right = couple.begin()->second.getMaxPos();
    vector<double> initposes;
    vector<double> s;
    double sums = 0;
    cout<<"Couplename    : "<<couple.begin()->second.getCoupleName()<<endl;
    cout<<"Trying to find a matano plane for this couple!"<<endl;
    for(iter=couple.begin(); iter!=couple.end(); ++iter)
    {
        double minpos = iter->second.getMinPos();
        double maxpos = iter->second.getMaxPos();
        if(left<minpos)
        {
            left = minpos;
        }
        if(right>maxpos)
        {
            right = maxpos;
        }
        double tinitpos = iter->second.getInitPos();
        initposes.push_back(tinitpos);
        double ts = iter->second.getSumS();
        cout<<"Integral area of   "<<iter->second.getElementName()<<" : ";
        cout<<ts<<endl;
        cout<<"Matano plane of    "<<iter->second.getElementName()<<" : ";
        cout<<tinitpos<<endl;
        s.push_back(ts);
        sums += ts;
    }

    posleft = left;
    posright = right;

    initpos = 0;
    int pn = s.size();
    for(int i = 0; i<pn; i++)
    {
        initpos += initposes[i]*s[i]/sums;
    }
    cout<<"Decisive matano position: "<<initpos<<endl;

    is_getinitpos = 1;

    return initpos;
}

void Couple::makeInterpolated(double _dx)
{
    if(is_interpolated) return;
    if(!is_getinitpos) getInitPos();
    if(dx>0.0) _dx = dx;
    dx = _dx;
    //Set the spacing step for the interpolated couple data
    //getInitPos();
    //get initial position and the values for posleft, posright etc.

    std::map<std::string, std::vector<double> > tcouple;
    map<string, Profile>::iterator iter;

    int N = (posright - posleft)/dx;
    
    for(auto name=elementnames.begin(); name!=elementnames.end(); ++name)
    {
        SplinesLoad::CubicSpline spline;
        //use the cubic spline from the SplinesLoad space
        spline.build(couple[*name].distance, 
                     couple[*name].concentration);
        
        double x = posleft;
        vector<double> tmp;
        while(x<=posright)
        {
            tmp.push_back(spline(x));
            x += dx;
        }
        if(int(tmp.size()) < N)
        {
            N = tmp.size();
        }
        pair<string, vector<double> > pair =
            make_pair(*name, tmp);
        tcouple.insert(pair);
    }
    
    mcouple.Allocate(N, 1, 1, couple.size());
    for(int nx=0; nx<N; ++nx)
    {
        int ci = 0;
        for(auto name=elementnames.begin(); name!=elementnames.end(); ++name, ++ci)
        {
            mcouple(nx, 0, 0, ci) = tcouple[*name][nx];
        }
    }
    is_interpolated = 1;
}

string Couple::getCoupleName()
{
    return couplename;
}

void Couple::makeInfo(Info &info)
{
    if(!is_getinitpos) getInitPos();
    if(!is_interpolated) makeInterpolated();

    info.Couplename = couple.begin()->second.getCoupleName();

    info.Nx = int((posright-posleft)/dx);
    info.Ny = 1;
    info.Nz = 1;
    info.Time = couple.begin()->second.getTime();
    info.Temperature = couple.begin()->second.getTemperature();

    map<string, Profile>::iterator iter;
    for(auto name=elementnames.begin(); name!=elementnames.end(); ++name)
    {
        info.Symbols.push_back(*name);

        pair<string, double> pair_cleft = \
            make_pair(*name, couple[*name].getCleft());
        pair<string, double> pair_cright = \
            make_pair(*name, couple[*name].getCright());
        info.Cleft.insert(pair_cleft);
        info.Cright.insert(pair_cright);

        pair<string, int> pair_pos = \
            make_pair(*name, int((initpos-posleft)/dx));
        //the grid position will be returned.
        info.Position.insert(pair_pos);
    }
}

map<string, Couple> Couple::read(string filepath)
{
    Profile tmp;
    vector<Profile> profiles = tmp.read(filepath);
    set<string> couplenames;
    vector<Profile>::iterator iter;
    for(iter=profiles.begin(); iter<profiles.end(); ++iter)
    {
        couplenames.insert(iter->getCoupleName());
    }
    set<string>::iterator jter;
    map<string, Couple> couples;
    for(jter=couplenames.begin(); jter!=couplenames.end(); ++jter)
    {
        Couple c(*jter);
        //cout<<*jter<<endl;
        vector<Profile> p;
        vector<Profile>::iterator kter;
        for(kter=profiles.begin(); kter<profiles.end(); kter++)
        {
            if(jter->compare(0, jter->size(), kter->getCoupleName()) == 0)
            {
                p.push_back(*kter);
            }
        }
        c.insert(p);
        c.show();
        pair<string, Couple> pair1 = make_pair(*jter, c);
        couples.insert(pair1);
    }
    return couples;
}
