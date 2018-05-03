#ifndef TARGET_H
#define TARGET_H

#include <string>

struct Target
{
    Target() {};
    Target(std::string n) : name(n) {};

    std::string name;

    std::vector<TH1D*> histo4M;
    std::vector<TH1D*> monitor4M;

    std::vector<TH1D*> histo6M;
    std::vector<TH1D*> monitor6M;
};

#endif /* TARGET_H */
