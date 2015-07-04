#include "info.h"

void Info::show()
{
    std::cout<<"CoupleName : "<<Couplename<<std::endl;
    std::cout<<"Time       : "<<Time<<std::endl;
    std::cout<<"Temperature: "<<Temperature<<std::endl;
    std::cout<<"PosLeft    : "<<PosLeft<<std::endl;
    std::cout<<"PosRigth   : "<<PosRight<<std::endl;
    std::cout<<"PosInit    : "<<PosInit<<std::endl;
    for(auto each=Symbols.begin(); each!=Symbols.end(); ++each)
    {
        std::cout<<*each<<"\t";
    }
    std::cout<<std::endl;
    for(auto each=Cleft.begin(); each!=Cleft.end(); ++each)
    {
        std::cout<<each->second<<"\t";
    }
    std::cout<<std::endl;
    for(auto each=Cright.begin(); each!=Cright.end(); ++each)
    {
        std::cout<<each->second<<"\t";
    }
    std::cout<<std::endl;
    
}
