#ifndef DETECTOR_H
#define DETECTOR_H

#include <string>

#include "TH1I.h"

struct Detector
{
    Detector(std::string n);
    void getThresholds(std::string thresholdFileName);

    std::string name;

    float pulseHeight;
    float PSD;
    float TDC;

    unsigned int pulseHeightThreshold;
    unsigned int PSDThreshold;
    unsigned int TDCLowThreshold;
    unsigned int TDCHighThreshold;

    TH1I* pulseHeightHisto;
    TH1I* PSDHisto;
    TH1I* TDCHisto;
};

#endif /* DETECTOR_H */
