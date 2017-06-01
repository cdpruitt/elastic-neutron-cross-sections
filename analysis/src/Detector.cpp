#include <iostream>
#include <fstream>

#include "../include/Detector.h"

using namespace std;

Detector::Detector(string n) : name(n)
{
    string PSDName = name + "PSD";
    string TDCName = name + "TDC";

    // read in thresholds for each variable
    string thresholdFileName = "configuration/thresholds/" + name + ".txt";
    ifstream file(thresholdFileName);

    if(!file.is_open())
    {
        cerr << "Error: failed to open " << thresholdFileName << " when attempting to read thresholds for " << name;
        exit(1);
    }

    string dummy;

    getline(file,dummy);

    file >> PSDThreshold;

    getline(file,dummy);
    getline(file,dummy);
    getline(file,dummy);

    file >> pulseHeightThreshold;

    getline(file,dummy);
    getline(file,dummy);
    getline(file,dummy);

    file >> TDCLowThreshold;
    
    getline(file,dummy);
    getline(file,dummy);
    getline(file,dummy);
 
    file >> TDCHighThreshold;

    file.close();

    // read in histogram specifications
    string histoFileName = "configuration/histos/" + name + ".txt";
    ifstream histoFile(histoFileName);

    if(!histoFile.is_open())
    {
        cerr << "Error: failed to open " << histoFileName << " when attempting to read thresholds for " << name;
        exit(1);
    }

    getline(histoFile,dummy);

    unsigned int PSD_MINIMUM;
    unsigned int PSD_MAXIMUM;
    histoFile >> PSD_MINIMUM >> PSD_MAXIMUM;

    PSDHisto = new TH1I(PSDName.c_str(), PSDName.c_str(),
            PSD_MAXIMUM-PSD_MINIMUM, PSD_MINIMUM, PSD_MAXIMUM);

    getline(histoFile,dummy);
    getline(histoFile,dummy);
    getline(histoFile,dummy);

    unsigned int TDC_MINIMUM;
    unsigned int TDC_MAXIMUM;
    histoFile >> TDC_MINIMUM >> TDC_MAXIMUM;

    TDCHisto = new TH1I(TDCName.c_str(), TDCName.c_str(),
            TDC_MAXIMUM-TDC_MINIMUM, TDC_MINIMUM, TDC_MAXIMUM);

    histoFile.close();
}
