#ifndef REFERENCE_CS_H
#define REFERENCE_CS_H

#include <string>
#include <vector>

struct ReferenceCS
{
    ReferenceCS() {};
    ReferenceCS(std::string n) : name(n) {};

    std::string name;

    std::vector<double> counts;
    std::vector<double> monitors;

    double angle;
    double crossSection;
    double polyNumberOfAtoms;
    double graphiteNumberOfAtoms;
};

#endif /* REFERENCE_CS_H */
