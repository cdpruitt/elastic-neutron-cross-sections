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

    TH1D* pulseHeightHisto;
    TH1D* PSDHisto;
    TH1D* TDCHisto;

    TH1D* rawPH;
    TH1D* rawPSD;
    TH1D* rawTDC;

    TH1D* PHGatedPH;
    TH1D* PHGatedPSD;
    TH1D* PHGatedTDC;

    TH1D* PSDGatedPH;
    TH1D* PSDGatedPSD;
    TH1D* PSDGatedTDC;
};

#endif /* DETECTOR_H */
