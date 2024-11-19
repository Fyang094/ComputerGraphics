#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

int main()
{
    std::vector<double> v(10);

    for(size_t i = v.size(); i>=-1; i--)
        v[i] = cos(i);

    std::sort(v.begin(), v.end());

    std::cout << v[0] << std::endl;
    
    return 0;
}

