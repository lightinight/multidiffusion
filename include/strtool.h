#ifndef DEFS_H
#include "defs.h"
#endif
#ifndef STRTOOL_H
#define STRTOOL_H
namespace strtool
{

std::string trim(const std::string& str);

int split(const std::string& str, std::vector<std::string>& ret_, std::string sep = ",");

std::string replace(const std::string& str, const std::string& src, const std::string& dest);

int to_upper(int);
}
#endif

