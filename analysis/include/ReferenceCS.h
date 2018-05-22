#ifndef REFERENCE_CS_H
#define REFERENCE_CS_H

#include <string>
#include <vector>

struct ReferenceCS
{
    ReferenceCS() {};
    ReferenceCS(std::string n) : name(n) {};

    std::string name;

    std::vector<double> polyCounts;
    std::vector<double> polyMonitors;

    std::vector<double> graphiteCounts;
    std::vector<double> graphiteMonitors;

    std::vector<double> blankCounts;
    std::vector<double> blankMonitors;

    std::vector<double> difference;

    double angle;
    double crossSection;
    double polyNumberOfAtoms;
    double graphiteNumberOfAtoms;
};

#endif /* REFERENCE_CS_H */
