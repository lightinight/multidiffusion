#ifndef DEFS_H
#include "defs.h"
#endif

#ifndef STORAGES_H
#include "storages.h"
#endif

#ifndef STRTOOL_H
#include "strtool.h"
#endif

#ifndef IO_H
#define IO_H

template <class T>
class IO
{
    /*This class maitain the input and output operations*/
public:

    std::string readAndRemoveAnnotation(std::string InputFileName);

    void parseTypeOne(std::string inbufer,
        std::map<std::string, std::string> &dict_infos,
        std::map<std::string, std::string> &dict_items);

    void create(std::string pathname);

    void write(openphase::Storage3D<T, 0> &Field, std::string pathname, std::string info="");

    void writeVTK(std::string outputfilename, openphase::Storage3D<double, 1> &var);
    void writeVTK(std::string outputfilename, openphase::Storage3D<double, 2> &var);
};
#endif

