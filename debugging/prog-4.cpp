#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

void print_cases(const std::string& name)
{
    // change up and lo to allow for name sizes more than 8
    char* up = new char[name.size() + 1];
    char* lo = new char[name.size() + 1];

    for(size_t i = 0; i < name.size(); i++)
    {
        up[i] = std::toupper(name[i]);
        lo[i] = std::tolower(name[i]);
    }

    // add null-terminators to make into valid c-strings
    up[name.size()] = '\0';
    lo[name.size()] = '\0';

    std::cout << lo << " " << up << std::endl;

    // deleted up and lo to avoid memory leaks
    delete[] up;
    delete[] lo;
}

int main()
{
    print_cases("Elizabeth");
    print_cases("John");
    print_cases("Frank");
    print_cases("Fred");

    return 0;
}