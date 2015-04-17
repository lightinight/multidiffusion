#include "diffreader.h"
using namespace std;
namespace strtool
{

string trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}


int split(const string& str, vector<string>& ret_, string sep = ",")
{
    if (str.empty())
    {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos)
    {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos)
        {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        }
        else
        {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty())
        {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

string replace(const string& str, const string& src, const string& dest)
{
    string ret;

    string::size_type pos_begin = 0;
    string::size_type pos       = str.find(src);
    while (pos != string::npos)
    {
        //cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
        ret.append(str.data() + pos_begin, pos - pos_begin);
        ret += dest;
        pos_begin = pos + 1;
        pos       = str.find(src, pos_begin);
    }
    if (pos_begin < str.length())
    {
        ret.append(str.begin() + pos_begin, str.end());
    }
    return ret;
}

}


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
			if(line.empty())
			{
				cout<<"line "<<linenum<<" was left out!"<<endl;
				continue;
			}
			int notation = line.find_first_not_of(" ", 0);
			if(line.compare(notation, 1, "#")==0)
			{
				cout<<"line "<<linenum<<" was commented away!"<<endl;
				continue;
			}
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
	
	vector<string> result;
	strtool::split(inbufer, result, "DATA");
	
	vector<Profile> profiles;
	vector<string>::iterator iter;
	for( iter = result.begin(); iter<result.end(); iter++)
	{
	    //cout<<"lokkkkkkkkkkkkkkkkkkkkkkkkkkkk"<< *iter<<endl;
	    if(iter->size() <= 1) continue;
		Profile profile = splitData(*iter);
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
    Profile profile(_couplename, _elementname, _temperature, _time, _distance, _concentration, _cleft, _cright);
    //profile.show();
    return profile;
}

void Profile::sort()
{
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
	cout<<"Couple Name: "<<couplename<<endl;
	cout<<"Element Name: "<<elementname<<endl;
	cout<<"Time(min): "<<time<<endl;
	cout<<"Temperature(K): "<<temperature<<endl;
	cout<<"Cleft: "<<cleft<<endl;
	cout<<"Cright: "<<cright<<endl;
	cout<<"Initial position: "<<getInitPos()<<endl;
	cout<<"concentration:";
	int nsize = distance.size();
	for(int i=0; i<nsize; i++)
	{
	    cout<<concentration[i]<<" ";
	}
	cout<<"\t"<<concentration.size()<<endl;
	
	cout<<"distance:";
	for(int i=0; i<nsize; i++)
	{
	    cout<<distance[i]<<" ";
	}
	cout<<"\t"<<distance.size()<<endl;
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

Couple::Couple(string _couplename, vector<Profile> profiles)
{
    couplename = _couplename;
    vector<Profile>::iterator iter;
    for(iter=profiles.begin(); iter<profiles.end(); iter++)
    {
        pair<string, Profile> pair1 = \
            make_pair(iter->getElementName(), *iter);
        couple.insert(pair1);
    } 
}

Couple::Couple(string _couplename)
{
    couplename = _couplename;
}

void Couple::insert(vector<Profile> profiles)
{
    vector<Profile>::iterator iter;
    for(iter=profiles.begin(); iter<profiles.end(); iter++)
    {
        iter->sort();
        iter->smooth(3);
        pair<string, Profile> pair1 = \
            make_pair(iter->getElementName(), *iter);
        couple.insert(pair1);
    } 
}

void Couple::show()
{
    map<string, Profile>::iterator iter;
    cout<<"\ncouplename: "<<couplename<<endl;
    for(iter=couple.begin(); iter!=couple.end(); iter++)
    {
        iter->second.show();
    }
}

void Couple::makeInitCon(double dx)
{
    map<string, Profile>::iterator iter;
    double left = 0;
    double right = couple.begin()->second.getMaxPos();
    vector<double> initposes;
    vector<double> s;
    double sums = 0;
    for(iter=couple.begin(); iter!=couple.end(); iter++)
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
        initposes.push_back(iter->second.getInitPos());
        double ts = iter->second.getSumS();
        s.push_back(ts);
        sums += ts;
    }
    double initpos = 0;
    int pn = s.size();
    for(int i = 0; i<pn; i++)
    {
        initpos += initposes[i]*s[i]/sums;
    }
    
    int N = (right - left)/dx;
    int Nr = (right - initpos)/dx;
    for(iter=couple.begin(); iter!=couple.end(); iter++)
    {
        vector<double> tc(N, iter->second.getCleft());
        double _cright = iter->second.getCright();
        for(int i=Nr; i<N; i++)
        {
            tc[i] = _cright;
        }
        pair<string, vector<double> > pair1 = \
            make_pair(iter->second.getElementName(), tc);
        mcouple.insert(pair1);
    }
}

map<string, Couple> diffreader(string filepath)
{
    Profile tmp;
    vector<Profile> profiles = tmp.read(filepath);
    set<string> couplenames;
    vector<Profile>::iterator iter;
    for(iter=profiles.begin(); iter<profiles.end(); iter++)
    {
        couplenames.insert(iter->getCoupleName());
    }
    set<string>::iterator jter;
    map<string, Couple> couples;
    for(jter=couplenames.begin(); jter!=couplenames.end(); jter++)
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
        pair<string, Couple> pair1 = make_pair(*jter, c);
        couples.insert(pair1);
    }
    return couples;
}
