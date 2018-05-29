#ifndef DETECTOR_EFFICIENCY_H
#define DETECTOR_EFFICIENCY_H

#include <vector>
#include <string>

struct DetectorEfficiency
{
    DetectorEfficiency() {};
    DetectorEfficiency(std::string detectorName);

    double getEfficiency(double energy) const;

    std::vector<double> energy;
    std::vector<double> efficiency;
};

#endif /* DETECTOR_EFFICIENCY_H */
