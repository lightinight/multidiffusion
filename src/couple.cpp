#include "couple.h"
using namespace std;
void Couple::presetting()
{
    is_getinitpos = 0;
}

Couple::Couple()
{
    presetting();
}

Couple::Couple(string _couplename)
{
    presetting();
    couplename = _couplename;
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

Couple::Couple(const Couple& obj)
{
    presetting();
    couple.insert(obj.couple.begin(), obj.couple.end());
    if(obj.is_getinitpos)
    {
        couplename = obj.couplename;
        elementnames.clear();
        for(auto each=obj.elementnames.begin(); each!=obj.elementnames.end(); ++each)
        {
            elementnames.push_back(*each);
        }
        initpos = obj.initpos;
        posleft = obj.posleft;
        posright = obj.posright;
        is_getinitpos = 1;
    }
    else
    {
        getInitPos();
    }
}

Couple& Couple::operator=(const Couple &rhs)
{
    if(this != &rhs)
    {
        couple.clear();
        presetting();
        couple.insert(rhs.couple.begin(), rhs.couple.end());
        if(rhs.is_getinitpos)
        {
            couplename = rhs.couplename;
            elementnames.clear();
            for(auto each=rhs.elementnames.begin(); each!=rhs.elementnames.end(); ++each)
            {
                elementnames.push_back(*each);
            }
            initpos = rhs.initpos;
            posleft = rhs.posleft;
            posright = rhs.posright;
            is_getinitpos = 1;
        }
        else
        {
            getInitPos();
        }
    }
    return *this;
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

double Couple::getInitPos()
{
/*initpos posleft posright is_getinitpos*/
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

void
Couple::makeInterpolated(double _dx)
{
/*won't call until makeInfo or manually call by the user*/
/*Strongly commanded that this function should be called manually.*/
    if(!is_getinitpos) getInitPos();

    map<string, Profile>::iterator iter;

    int N = int((posright - posleft)/_dx) + 1;
    
    mconc.Clear();
    mconc.Reallocate(N, 1, 1, elementnames.size());
    
    int index = 0;
    for(auto name=elementnames.begin(); name!=elementnames.end(); ++index, ++name)
    {
        SplinesLoad::CubicSpline spline;
        //use the cubic spline from the SplinesLoad space
        spline.build(couple[*name].getDistance(), 
                     couple[*name].getConcentration());
        
        double x = posleft;
        for(int nx=0; nx<N; ++nx)
        {
            x += nx*_dx;
            mconc(nx, 0, 0, index) = spline(x);
        }
    }
}

string Couple::getCoupleName()
{
    return couplename;
}

void Couple::makeInfo(Info &info)
{
    if(!is_getinitpos) getInitPos();

    info.Couplename = couple.begin()->second.getCoupleName();

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
    }
    info.PosLeft = posleft;
    info.PosRight = posright;
    info.PosInit = initpos;
}

openphase::Storage3D<double, 1>& Couple::getMconc()
{
    return mconc;
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
        //c.show();
        pair<string, Couple> pair1 = make_pair(*jter, c);
        couples.insert(pair1);
    }
    return couples;
}

