#ifndef REFERENCE_CS_H
#define REFERENCE_CS_H

#include <string>
#include <vector>

struct ReferenceCS
{
    ReferenceCS() {};
    ReferenceCS(std::string n) : name(n) {};

    std::string name;

    double polyCounts;
    double polyMonitors;

    double graphiteCounts;
    double graphiteMonitors;

    double blankCounts;
    double blankMonitors;

    double difference;

    double angle;
    double crossSection;
    double polyNumberOfAtoms;
    double graphiteNumberOfAtoms;

    std::vector<double> intLimits;
};

#endif /* REFERENCE_CS_H */
