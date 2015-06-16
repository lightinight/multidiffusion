#include "profile.h"
using namespace std;

Profile::Profile(const Profile& profile)
{
    distance.clear();
    distance.clear();
    couplename = profile.couplename;
    elementname = profile.elementname;
    time = profile.time;
    temperature = profile.temperature;
    copy(profile.distance.begin(), profile.distance.end(), back_inserter(distance));
    copy(profile.concentration.begin(), profile.concentration.end(), back_inserter(concentration));
    cleft = profile.cleft;
    cright = profile.cright;
}

Profile::Profile(string _couplename, string _elementname, double _time, double _temperature, vector<double> _distance, vector<double> _concentration, double _cleft, double _cright)
{
    couplename = _couplename;
    elementname = _elementname;
    time = _time;
    temperature = _temperature;
    copy(_distance.begin(), _distance.end(), back_inserter(distance));
    copy(_concentration.begin(), _concentration.end(), back_inserter(concentration));
    cleft = _cleft;
    cright = _cright;
    //spline.build(distance, concentration);
}

vector<Profile> Profile::read(string filepath)
{
    ifstream infile;
    infile.open(filepath.c_str());
    if(!infile)
    {
        std::cout<<"Fail to open:"<<filepath<<std::endl;
        cout<<"This program will be terminated!"<<endl;
        exit(0);
    }

    char buf[1024];
    string inbufer = " ";

    int linenum = 0;
    if(infile.is_open())
    {
        while(infile.good() && !infile.eof())
        {
            memset(buf,0,1024);
            infile.getline(buf,1024);
            string line = buf;
            //deal with empty line
            if(line.empty())
            {
                cout<<"line "<<linenum<<" was left out!"<<endl;
                continue;
            }
            //deal with a line begin with '#'
            int notation = line.find_first_not_of(" ", 0);
            if(line.compare(notation, 1, "#")==0)
            {
                cout<<"line "<<linenum<<" was commented away!"<<endl;
                continue;
            }
            //deal with a code line with annotation at the end
            vector<string> tmp;
            strtool::split(line, tmp, "#");
            if(tmp.size()>1)
            {
                line = tmp[0];
            }

            line = strtool::trim(line);

            vector<string> lines;
            string t = "a";
            strtool::split(line, lines, " ");
            vector<string>::iterator jter;
            for(jter = lines.begin(); jter<lines.end(); ++jter)
            {
                inbufer += *jter;
                inbufer += " ";
            }
            //inbufer += line;
            linenum++;
        }
    }
    infile.close();
    infile.close();

    vector<string> result;
    strtool::split(inbufer, result, "DATA");

    vector<Profile> profiles;
    vector<string>::iterator iter;
    for( iter = result.begin(); iter<result.end(); iter++)
    {
        //cout<<"lokkkkkkkkkkkkkkkkkkkkkkkkkkkk"<< *iter<<endl;
        if(iter->size() <= 1) continue;
        Profile profile = splitData(*iter);
        //profile.show();
        profile.sort();
        profiles.push_back(profile);
    }
    /*
    vector<Profile>::iterator jter;
    for(jter=profiles.begin(); jter<profiles.end(); jter++)
    {
        jter->show();
    }*/
    return profiles;
}

Profile Profile::splitData(string dataset_str)
{
    dataset_str = strtool::replace(dataset_str, "}", "");
    vector<string> dataset;
    strtool::split(dataset_str, dataset, "{");
    string info = strtool::replace(dataset[0], ")", "");
    info = strtool::replace(info, "(", "");
    vector<string> key_keyword;
    strtool::split(info, key_keyword, ",");
    vector<string>::iterator iter;

    string _couplename = "";
    string _elementname = "";
    double _time = 0;
    double _temperature = 0;
    double _cleft = 0;
    double _cright = 0;
    vector<double> _distance;
    vector<double> _concentration;

    for(iter=key_keyword.begin(); iter<key_keyword.end(); iter++)
    {
        vector<string> item;
        strtool::split(strtool::trim(*iter), item, "=");
        string key = item[0];
        transform(key.begin(), key.end(), key.begin(), towupper);
        string word = strtool::trim(item[1]);

        std::string::size_type sz;
        if(key.compare(0, key.size(), "COUPLE")==0)
        {
            transform(word.begin(), word.end(), word.begin(), towupper);
            _couplename = word;
            //cout<<"get couplename:"<<_couplename<<endl;
        }
        if(key.compare(0, key.size(), "ELEMENT")==0)
        {
            transform(word.begin(), word.end(), word.begin(), towupper);
            _elementname = word;
        }
        if(key.compare(0, key.size(), "TIME")==0)
        {
            _time = stof(word);
        }
        if(key.compare(0, key.size(), "TEMPERATURE")==0)
        {
            _temperature = stof(word);
        }
        if(key.compare(0, key.size(), "CLEFT")==0)
        {
            _cleft = stof(word);
        }
        if(key.compare(0, key.size(), "CRIGHT")==0)
        {
            _cright = stof(word, &sz);
        }
        //cout<<" word "<<word;
    }
    //cout<<_couplename<<" "<<_elementname<<" "<<_temperature<<" "<<_time<<" "<<_cleft<<" "<<_cright<<endl;

    vector<string> data;
    strtool::split(dataset[1], data, " ");
    vector<string>::iterator jter;
    int count = 0;
    for(jter=data.begin(); jter<data.end(); jter++, count++)
    {
        double floatnumber = stof(*jter);
        if(count%2 == 0)
        {
            _distance.push_back(floatnumber);
        }
        else
        {
            _concentration.push_back(floatnumber);
        }
    }
    Profile profile(_couplename, _elementname, _time, _temperature, _distance, _concentration, _cleft, _cright);
    //profile.show();
    return profile;
}

void Profile::sort()
{
    cout<<couplename<<"    "<<elementname<<"    "<<"soted"<<endl;
    int N = distance.size();
    for(int i = 0; i<N; i++)
    {
        int tag = i;
        for(int j = i; j<N; j++)
        {
            if(distance[j] <= distance[tag])
            {
                tag = j;
            }
        }
        if(tag != i)
        {
            double td = distance[i];
            distance[i] = distance[tag];
            distance[tag] = td;
            double tc = concentration[i];
            concentration[i] = concentration[tag];
            concentration[tag] = tc;
        }
    }
}

Profile Profile::smooth(int times)
{
//TODO: the algorithm should be changed, the concentration should be weighted with the distance!
    int N = distance.size();
    vector<double> nconcentration;
    for(int i = 0; i<times/2; i++)
    {
        nconcentration.push_back(concentration[i]);
    }
    for(int i = times/2; i<N-times/2; i++)
    {
        double nc = 0;
        for(int j=0; j<times; j++)
        {
            nc += concentration[i+j-times/2]/double(times);
        }
        nconcentration.push_back(nc);
    }
    for(int i = N - times/2; i<N; i++)
    {
        nconcentration.push_back(concentration[i]);
    }
    cout<<couplename<<" "<<N<<" check smoothed array size"<<nconcentration.size()<<endl;
    Profile profile(*this);
    profile.concentration = nconcentration;
    return profile;
}

void Profile::show()
{
    cout<<"Couple Name      : "<<couplename<<endl;
    cout<<"Element Name     : "<<elementname<<endl;
    cout<<"Time(min)        : "<<time<<endl;
    cout<<"Temperature(K)   : "<<temperature<<endl;
    cout<<"Cleft(at.%)      : "<<cleft<<endl;
    cout<<"Cright(at.%)     : "<<cright<<endl;
    cout<<"Posleft          : "<<getMinPos()<<endl;
    cout<<"Posright         : "<<getMaxPos()<<endl;
    cout<<"Matano Plane     : "<<getInitPos()<<endl;
    int nsize = distance.size();
    cout<<"Total data ponits: "<<nsize<<endl;
    cout<<"--------------D A T A---------------"<<endl;
    cout<<"   distance         concentration"<<endl;
    cout<<" *1.0e-6 meter          at.%     "<<endl;
    for(int i=0; i<5; i++)
    {
        cout<<"   "<<distance[i]<<"           ";
        cout<<concentration[i]<<endl;
    }
    cout<<"   ......              ......    "<<endl;
    for(int i=nsize-6; i<nsize; i++)
    {
        cout<<"   "<<distance[i]<<"           ";
        cout<<concentration[i]<<endl;
    }
    cout<<endl;
}

double Profile::getInitPos()
{
    //show();

    int dsize = distance.size();
    int csize = concentration.size();
    if(dsize != csize)
    {
        cout<<"the size of d and c are not equal!"<<endl;
        return 0;
    }
    //cout<<"distance size:"<<pnum<<endl;
    double s = 0;
    double ld = distance[0];
    double rd = distance[dsize-1];
    for(int i=0; i<dsize-1; i++)
    {
        s += (distance[i+1] - distance[i]) \
           * (concentration[i] + concentration[i+1])/2.0;
    }
    //cout<<"divided "<<cleft<<" "<<cright<<endl;
    return (s + cleft*ld - cright*rd) / (cleft - cright);
}

double Profile::getSumS()
{
    int dsize = distance.size();
    int csize = concentration.size();
    if(dsize != csize)
    {
        cout<<"the size of d and c are not equal!"<<endl;
        return 0;
    }
    //cout<<"distance size:"<<pnum<<endl;
    double s = 0;
    for(int i=0; i<dsize-1; i++)
    {
        s += (distance[i+1] - distance[i]) \
           * (concentration[i] + concentration[i+1])/2.0;
    }
    return s;
}

double Profile::getMinPos()
{
    return distance[0];
}

double Profile::getMaxPos()
{
    return distance[distance.size()-1];
}

double Profile::getTime()
{
    return time;
}

double Profile::getTemperature()
{
    return temperature;
}

double Profile::getCleft()
{
    return cleft;
}

double Profile::getCright()
{
    return cright;
}

string Profile::getCoupleName()
{
    return couplename;
}

string Profile::getElementName()
{
    return elementname;
}

/*
SplinesLoad::CubicSpline &Profile::getspline()
{
    return spline;
}*/
