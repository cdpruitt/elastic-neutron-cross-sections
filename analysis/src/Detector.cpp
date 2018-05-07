#include <iostream>
#include <fstream>

#include "../include/Detector.h"
#include "../include/Config.h"

using namespace std;

Detector::Detector(string n, string experiment) : name(n)
{
    string PHName = name + "PH";
    string PSDName = name + "PSD";
    string TDCName = name + "TDC";
    string PHPSDName = name + "PHPSD";

    pulseHeightHisto = new TH1D(PHName.c_str(), PHName.c_str(),
            5000, 0, 5000);

    PSDHisto = new TH1D(PSDName.c_str(), PSDName.c_str(),
            5000, 0, 5000);

    TDCHisto = new TH1D(TDCName.c_str(), TDCName.c_str(),
            5000, 0, 5000);

    PHPSD = new TH2D(PHPSDName.c_str(), PHPSDName.c_str(),
            1000, 0, 5000,
            1000, 0, 5000);

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

    string bananaFileName = "../configuration/" + experiment + "/PSDGates/" + name + "Banana.data";
    banana = PIDBanana(bananaFileName);
}

void Detector::write()
{
    rawPH->Write();
    rawPSD->Write();
    rawTDC->Write();
    rawPHPSD->Write();

    pulseHeightHisto->Write();
    PSDHisto->Write();
    TDCHisto->Write();
    PHPSD->Write();
}
