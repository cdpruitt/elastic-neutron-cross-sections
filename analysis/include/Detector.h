#ifndef DETECTOR_H
#define DETECTOR_H

#include <string>

#include "PIDBanana.h"
#include "DetectorHistos.h"
#include "DetectorEfficiency.h"

struct Detector
{
    Detector(std::string n, std::string experiment);

    int getTDCBin(double neutronTOF);

    std::string name;

    float pulseHeight;
    float PSD;
    float TDC;

    PIDBanana banana;

    DetectorHistos histos;

    double linearCalibration; // in ns/bin
    double timeOffset; // in ns
    double resolution; // FWHM, in bins

    bool useForCS = false;

    DetectorEfficiency efficiency;

    double distance; // in cm
};

#endif /* DETECTOR_H */
