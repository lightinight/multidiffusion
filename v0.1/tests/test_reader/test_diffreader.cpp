#ifndef _DEFS_H
#include "defs.h"
#endif
#include "diffreader.h"
using namespace std;
int main()
{
	//cout<<"Input:";
	string filepath = "./test.diff";
	//cin>>filepath;
	/*
	Profile tmp;
	vector<Profile> profiles = tmp.read(filepath);
	cout<<"pass read"<<endl;
	vector<Profile>::iterator iter;
	for(iter=profiles.begin(); iter<profiles.end(); iter++)
	{
		//iter->show();
		double pos = iter->getInitPos();
		cout<<pos<<endl;
	}
	*/
	map<string, Couple> couples = diffreader(filepath);
	map<string, Couple>::iterator iter;
    for(iter=couples.begin(); iter!=couples.end(); iter++)
    {
        iter->second.show();
        iter->second.makeInitCon();
    }
	return 1;
}
