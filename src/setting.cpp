#include "setting.h"

using namespace std;

void Setting::show()
{

}

/*
void Setting::readInput(string InputFileName)
{
    //*read the information of the couples from the opi file!
    IO<double> inoperator;
    string inbufer =inoperator.readAndRemoveAnnotation(InputFileName);
    //cout<<inbufer<<endl;

    vector<string> result;
    strtool::split(inbufer, result, "COUPLE");
    //each item in result contains the setting for one couple
    for(auto each=result.begin(); each!=result.end(); ++each)
    {
        map<string, string> dict_infos, dict_items;
        //the infornamtion for a couple will be splited into two parts
        //preamble part and the content part
        inoperator.parseTypeOne(*each, dict_infos, dict_items);

        //for the preamble part
        int count = 0;
        vector<string> funcnames;
        Info info;
        for(auto item=dict_items.begin(); item!=dict_items.end(); ++item)
        {
            std::size_t found = item->first.find("&");
            if(found<0 || found>item->first.size())
            {
                pair<string, string> tpair = make_pair(item->first, item->second);
                info.Symbols.insert(tpair);
                info.Index.push_back(item->first);
                count += 1;
            }
            else
            {
                funcnames.push_back(item->first);
            }
        }

        for(auto jtem=info.Index.begin(); jtem!=info.Index.end(); ++jtem)
        {
            map<string, string> tmp;
            for(auto ktem=info.Index.begin(); ktem!=info.Index.end(); ++ktem)
            {
                for(auto ttem=funcnames.begins(); ttem!=funcnames.end(); ++ttem)
                {
                    vector<string> ttmp;
                    strtool::split(*ttem, ttmp, "&");
                    if(ttmp[0] == *ktem && ttmp[1] == *ttem)
                    {
                        pair<string, string> tpair = \
                            make_pair(*ktem, dict_items[*ttem]);
                        tmp.insert(tpair);
                        break;
                    }
                }
            }
            pair<string, map<string, string> > tpair = \
                make_pair(*jtem, tmp);
            info.IntrinsicDiffusivity.insert(tpair);
        }
        info.Cnums = count;

        pair<string, Info> pair1 = make_pair(dict_infos["NAME"], info);
        CoupleNames.push_back(dict_infos["NAME"]);

    }

}
*/
