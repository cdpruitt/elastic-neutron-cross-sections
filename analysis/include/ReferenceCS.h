#ifndef REFERENCE_CS_H
#define REFERENCE_CS_H

struct ReferenceCS
{
    std::vector<double> counts;
    std::vector<double> monitors;

    double angle;
    double crossSection;
    double polyNumberOfAtoms;
    double graphiteNumberOfAtoms;
};

#endif /* REFERENCE_CS_H */
