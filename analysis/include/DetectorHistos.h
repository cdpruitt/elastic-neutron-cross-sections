#ifndef DETECTOR_HISTOS_H
#define DETECTOR_HISTOS_H

#include "TH1.h"
#include "TH2.h"

#include <string>

struct DetectorHistos
{
    DetectorHistos() {};
    DetectorHistos(std::string name, double linearCalibration);
    void write();

    TH1D* pulseHeightHisto;
    TH1D* PSDHisto;
    TH1D* TDCHisto;
    TH1D* TOFHisto;
    TH2D* PHPSD;

    TH1D* rawPH;
    TH1D* rawPSD;
    TH1D* rawTDC;
    TH2D* rawPHPSD;
};

#endif /* DETECTOR_HISTOS_H */
