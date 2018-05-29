#include "../include/Detector.h"
#include "../include/DetectorHistos.h"

using namespace std;

DetectorHistos::DetectorHistos(string name, double linearCalibration)
{
    string PHName = name + "PH";
    string PSDName = name + "PSD";
    string TDCName = name + "TDC";
    string PHPSDName = name + "PHPSD";
    string TOFName = name + "TOF";

    pulseHeightHisto = new TH1D(PHName.c_str(), PHName.c_str(),
            5000, 0, 5000);

    PSDHisto = new TH1D(PSDName.c_str(), PSDName.c_str(),
            5000, 0, 5000);

    TDCHisto = new TH1D(TDCName.c_str(), TDCName.c_str(),
            5000, 0, 5000);

    PHPSD = new TH2D(PHPSDName.c_str(), PHPSDName.c_str(),
            1000, 0, 5000,
            1000, 0, 5000);

    double TOFHistoStart = 0;
    double TOFHistoStop = 500;
    int TOFHistoBins = ceil((TOFHistoStop-TOFHistoStart)/(linearCalibration));

    TOFHisto = new TH1D(TOFName.c_str(), TOFName.c_str(),
            TOFHistoBins, TOFHistoStart, TOFHistoStop);

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
}

void DetectorHistos::write()
{
    rawPH->Write();
    rawPSD->Write();
    rawTDC->Write();
    rawPHPSD->Write();

    pulseHeightHisto->Write();
    PSDHisto->Write();
    TDCHisto->Write();
    TOFHisto->Write();
    PHPSD->Write();
}
