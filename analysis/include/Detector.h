#ifndef DETECTOR_H
#define DETECTOR_H

#include <string>

#include "TH1.h"
#include "TH2.h"

#include "PIDBanana.h"

struct Detector
{
    Detector(std::string n, std::string experiment);
    void write();

    std::string name;

    float pulseHeight;
    float PSD;
    float TDC;

    TH1D* pulseHeightHisto;
    TH1D* PSDHisto;
    TH1D* TDCHisto;
    TH2D* PHPSD;

    TH1D* rawPH;
    TH1D* rawPSD;
    TH1D* rawTDC;
    TH2D* rawPHPSD;

    PIDBanana banana;
};

#endif /* DETECTOR_H */
