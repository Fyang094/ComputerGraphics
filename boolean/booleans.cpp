#include "common.h"

// Given two hit lists (a and b), compute and return a corresponding hit list
// for the boolean operation indicated by type (type_intersect, type_union, or
// type_difference)
std::vector<Hit> Combine(
    const std::vector<Hit>& a,
    const std::vector<Hit>& b,
    bool_type type)
{
    std::vector<Hit> x = {};

    std::cout << "sizes: a - " << a.size() << " b - " << b.size() << std::endl;

    if(type == type_intersect){
       for (const auto& hitA : a) {
                for (const auto& hitB : b) {
                    if (hitA.dist == hitB.dist) {
                        x.push_back(hitA);
                        break;
                    }
                }
            }
    }

    if(type == type_union){
        x = a;
            for (const auto& hitB : b) {
                bool found = false;
                for (const auto& hitX : x) {
                    if (hitB.dist == hitX.dist) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    x.push_back(hitB);
                }
            }
    }

    if(type == type_difference){
        x = a;
            for (const auto& hitB : b) {
                auto it = std::find_if(x.begin(), x.end(), [&](const Hit& hitX) {
                    return hitB.dist == hitX.dist;
                });
                if (it != x.end()) {
                    x.erase(it);
                }
            }
    }

    //sort(x);

    return x;
}

// Returns whether a point at distance t along a ray is inside an object whose
// hit list is given in a.
bool Test_Inside(const std::vector<Hit>& a, double t)
{   
    //std::cout << "t " << t << std::endl;

    if(a.size() == 1){
        if((a[0].dist <= t) )
            return true;
    } else {
        for (unsigned int i = 0; i < a.size(); i++){
            for (unsigned int j = a.size(); j > 0; j--){
            //std::cout << a[i] << std::endl;
            if((a[i].dist <= t) && (a[j].dist >= t) && (a[i].dist != a[j].dist))
                return true;
            }
        }
    }

    return false;
}
