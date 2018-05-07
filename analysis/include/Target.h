#ifndef TARGET_H
#define TARGET_H

#include <string>
#include "TH1.h"

struct Target
{
    Target() {};
    Target(std::string n) : name(n) {};

    std::string name;

    std::vector<std::vector<TH1D*>> histos;
    std::vector<std::vector<TH1D*>> monitors;
};

#endif /* TARGET_H */
