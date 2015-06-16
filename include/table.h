#ifndef _DEFS_H
#include "defs.h"
#endif
#ifndef TABLE_H
#define TABLE_H
template <typename T1, typename T2>
class Table
{
private:
    std::map<int, std::map<int, T1> > keys; 
    std::vector<T2> values;
public:
    Table() {;}
    Table(std::map<int, std::map<int, T1> > _keys, std::vector<T2> _values)
    {
        keys = _keys;
        values = _values;
    }
    Table(const Table &table)
    {
        keys = table.keys;
        values = table.values;
    }
    void show()
    {
        for(auto item=keys.begin(); item!=keys.end(); ++item)
        {
            for(auto key=item->second.begin(); key!=item->second.end(); ++key)
            {
                std::cout<<key->second<<" ";
            }
            std::cout<<std::endl;
        }
    }
    T2 operator()(std::vector<T1> key)
    {
        size_t index = 0;
        int flag = 0;
        for(index=0; index<keys.size(); ++index)
        {
            std::map<int, T1> eachkey = keys[index];
            if(key.size() != eachkey.size())
            {
                break;
            }
            size_t count = 0;
            for(size_t j=0; j<key.size(); ++j)
            {
                if(key[j] == eachkey[j]) count++;
            }
            if(count == key.size())
            {
                
                flag = 1;
                break;
            }
        }
        if(flag)
        {
            return values[index];
        }
        else
        {
            std::cout<<"Failed to execute, keyword error!"<<std::endl;
            show();
            exit(1);
        }
    }
};
#endif

