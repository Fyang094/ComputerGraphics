#ifndef __COMMON__
#define __COMMON__

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

struct Hit
{
    std::string object;
    double dist;
};

enum bool_type{type_intersect,type_union,type_difference};

std::vector<Hit> Combine(
    const std::vector<Hit>& a,
    const std::vector<Hit>& b,
    bool_type type);

bool Test_Inside(const std::vector<Hit>& a, double t);

inline void sort(std::vector<Hit>& hits)
{
    std::sort(hits.begin(),hits.end(),[](auto&a,auto&b){return a.dist<b.dist;});
}

inline std::ostream& operator<<(std::ostream& o, const Hit& h)
{
    return o << "(" << h.object << " " << h.dist << ")";
}

inline std::ostream& operator<<(std::ostream& o, const std::vector<Hit>& h)
{
    o << "( ";
    for(const auto& i:h) o << i << " ";
    return o << ")";
}

#endif
