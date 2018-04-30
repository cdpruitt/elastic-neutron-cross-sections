#include <iostream>
#include <fstream>

#include "../include/Detector.h"

using namespace std;

Detector::Detector(string n) : name(n)
{
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
    string histoFileName = "configuration/histoBounds/" + name + ".txt";
    ifstream histoFile(histoFileName);

    if(!histoFile.is_open())
    {
        cerr << "Error: failed to open " << histoFileName << " when attempting to read thresholds for " << name;
        exit(1);
    }

    string PHName = name + "PH";
    string PSDName = name + "PSD";
    string TDCName = name + "TDC";
 
    getline(histoFile,dummy);

    unsigned int PH_MINIMUM;
    unsigned int PH_MAXIMUM;
    histoFile >> PH_MINIMUM >> PH_MAXIMUM;

    pulseHeightHisto = new TH1D(PHName.c_str(), PHName.c_str(),
            PH_MAXIMUM-PH_MINIMUM, PH_MINIMUM, PH_MAXIMUM);

    getline(histoFile,dummy);
    getline(histoFile,dummy);
    getline(histoFile,dummy);

    unsigned int PSD_MINIMUM;
    unsigned int PSD_MAXIMUM;
    histoFile >> PSD_MINIMUM >> PSD_MAXIMUM;

    PSDHisto = new TH1D(PSDName.c_str(), PSDName.c_str(),
            PSD_MAXIMUM-PSD_MINIMUM, PSD_MINIMUM, PSD_MAXIMUM);

    getline(histoFile,dummy);
    getline(histoFile,dummy);
    getline(histoFile,dummy);

    unsigned int TDC_MINIMUM;
    unsigned int TDC_MAXIMUM;
    histoFile >> TDC_MINIMUM >> TDC_MAXIMUM;

    TDCHisto = new TH1D(TDCName.c_str(), TDCName.c_str(),
            TDC_MAXIMUM-TDC_MINIMUM, TDC_MINIMUM, TDC_MAXIMUM);

    histoFile.close();

    string rawPHName = PHName + "raw";
    string rawPSDName = PSDName + "raw";
    string rawTDCName = TDCName + "raw";
    string rawPHPSDName = name + "PHPSDraw";
    
    rawPH = new TH1D(rawPHName.c_str(), rawPHName.c_str(),
            5000, 0, 5000);

    rawPSD = new TH1D(rawPSDName.c_str(), rawPSDName.c_str(),
            5000, 0, 5000);

    rawTDC = new TH1D(rawTDCName.c_str(), rawTDCName.c_str(),
            5000, 0, 5000);

    rawPHPSD = new TH2D(rawPHPSDName.c_str(), rawPHPSDName.c_str(),
            1000, 0, 5000,
            1000, 0, 5000);


    string PHGatedPHName = PHName + "PHGated";
    string PHGatedPSDName = PSDName + "PHGated";
    string PHGatedTDCName = TDCName + "PHGated";

    PHGatedPH = new TH1D(PHGatedPHName.c_str(), PHGatedPHName.c_str(),
            5000, 0, 5000);

    PHGatedPSD = new TH1D(PHGatedPSDName.c_str(), PHGatedPSDName.c_str(),
            5000, 0, 5000);

    PHGatedTDC = new TH1D(PHGatedTDCName.c_str(), PHGatedTDCName.c_str(),
            5000, 0, 5000);

    string PSDGatedPHName = PHName + "PSDGated";
    string PSDGatedPSDName = PSDName + "PSDGated";
    string PSDGatedTDCName = TDCName + "PSDGated";

    PSDGatedPH = new TH1D(PSDGatedPHName.c_str(), PSDGatedPHName.c_str(),
            5000, 0, 5000);

    PSDGatedPSD = new TH1D(PSDGatedPSDName.c_str(), PSDGatedPSDName.c_str(),
            5000, 0, 5000);

    PSDGatedTDC = new TH1D(PSDGatedTDCName.c_str(), PSDGatedTDCName.c_str(),
            5000, 0, 5000);

    string PHPSDName = name + "PHPSD";
    PHPSD = new TH2D(PHPSDName.c_str(), PHPSDName.c_str(),
            1000, 0, 5000,
            1000, 0, 5000);
}
