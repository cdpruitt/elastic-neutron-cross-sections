#include "TH1D.h"
#include "TFile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/experimentalConstants.h"
#include "../include/calculateReference.h"
#include "../include/ReferenceCS.h"
#include "../include/IntegrationLimits.h"
#include "../include/utilities.h"
#include "../include/Config.h"

extern Config config;

using namespace std;

void subtractBackground(
        TH1D* polyHisto,
        TH1D* polyMon,
        TH1D* graphiteHisto,
        TH1D* graphiteMon,
        TH1D* blankRefHisto,
        TH1D* blankRefMon,
        double TOF,
        ReferenceCS& reference,
        const Detector& d
        )
{
    double polyMonCounts = polyMon->GetEntries();
    double graphiteMonCounts = graphiteMon->GetEntries();
    double blankRefMonCounts = blankRefMon->GetEntries();

    polyHisto->Scale(NORMALIZATION_SCALING/polyMonCounts);
    polyHisto->Write();
    graphiteHisto->Scale(NORMALIZATION_SCALING/graphiteMonCounts);
    graphiteHisto->Write();
    blankRefHisto->Scale(NORMALIZATION_SCALING/blankRefMonCounts);
    blankRefHisto->Write();

    string polyMinusBlankName = "polyMinusBlank" + d.name;
    TH1D* polyMinusBlank = (TH1D*)polyHisto->Clone(polyMinusBlankName.c_str());
    polyMinusBlank->Add(blankRefHisto, -1);
    polyMinusBlank->Write();

    string graphiteMinusBlankName = "graphiteMinusBlank" + d.name;
    TH1D* graphiteMinusBlank = (TH1D*)graphiteHisto->Clone(graphiteMinusBlankName.c_str());
    graphiteMinusBlank->Add(blankRefHisto, -1);
    graphiteMinusBlank->Write();

    string diffHistoName = "polyMinusGraphite" + d.name;
    TH1D* diffHisto = (TH1D*)polyMinusBlank->Clone(diffHistoName.c_str());
    double molRatio = reference.polyNumberOfAtoms/reference.graphiteNumberOfAtoms;
    graphiteMinusBlank->Scale(molRatio);
    diffHisto->Add(graphiteMinusBlank, -1);
    diffHisto->Write();

    // perform integrals
    double TOFResolution = d.resolution*d.linearCalibration; // FWHM in ns
    int minIntBin = diffHisto->FindBin(TOF-TOFResolution);
    int maxIntBin = diffHisto->FindBin(TOF+TOFResolution);
    reference.counts.push_back(diffHisto->Integral(minIntBin, maxIntBin));
    reference.monitors.push_back(polyMonCounts);

    cout << "reference mb/sr per histo count/monitor count = " << reference.crossSection*(reference.monitors.back()/reference.counts.back()) << endl;
}

int calculateReference(string experiment, string setToUse, ReferenceCS& reference)
{
    string fileName = "../configuration/" + experiment + "/normalization/" + setToUse + ".txt";

    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cerr << "Failed to find reference CS file in " << fileName << std::endl;
        return 1;
    }

    int polyethyleneRun;
    int blankRefRun;
    int graphiteRun;

    string str;

    while(getline(file,str))
    {
        // ignore comments in data file
        string delimiter = " ";
        string token = str.substr(0,str.find(delimiter));

        // parse data lines into space-delimited tokens
        vector<string> tokens;
        istringstream iss(str);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        if(tokens.size()==0)
        {
            continue;
        }

        if(tokens[0] == "Polyethylene")
        {
            polyethyleneRun = stoi(tokens[3]);
        }

        else if(tokens[0] == "Blank")
        {
            blankRefRun = stoi(tokens[3]);
        }

        else if(tokens[0] == "Graphite")
        {
            graphiteRun = stoi(tokens[3]);
        }

        else if(tokens[0] == "Angle")
        {
            reference.angle = stod(tokens[3]);
        }

        else if(tokens[0] == "Literature")
        {
            reference.crossSection = stod(tokens[3]);
        }

        else if(tokens[0] == "Number" && tokens[3]=="poly:")
        {
            reference.polyNumberOfAtoms = stod(tokens[4]);
        }

        else if(tokens[0] == "Number" && tokens[3]=="graphite:")
        {
            reference.graphiteNumberOfAtoms = stod(tokens[4]);
        }
    }

    // open targets needed for calculateCS
    //string polyTargetLocation = "../configuration/" + experiment + "/normalization/" + "polyethylene.txt";
    //Target poly = Target(polyTargetLocation);

    //string blankRefTargetLocation = "../configuration/" + experiment + "/normalization/" + "blankRef.txt";
    //Target blankRef = Target(blankRefTargetLocation);

    //string graphiteTargetLocation = "../configuration/" + experiment + "/normalization/" + "graphite.txt";
    //Target graphite = Target(graphiteTargetLocation);

    // open histograms needed for calculating the counts needed for calculateCS
    string polyHistoLocation = "../processedData/" + experiment + "/runs/" + to_string(polyethyleneRun) + "/histos.root";
    TFile* polyFile = new TFile(polyHistoLocation.c_str(), "READ");

    string blankRefHistoLocation = "../processedData/" + experiment + "/runs/" + to_string(blankRefRun) + "/histos.root";
    TFile* blankRefFile = new TFile(blankRefHistoLocation.c_str(), "READ");

    string graphiteHistoLocation = "../processedData/" + experiment + "/runs/" + to_string(graphiteRun) + "/histos.root";
    TFile* graphiteFile = new TFile(graphiteHistoLocation.c_str(), "READ");

    if(!polyFile->IsOpen() || !blankRefFile->IsOpen() || !graphiteFile->IsOpen())
    {
        cerr << "Error: could not open runs needed to calculate reference histogram counts." << endl;
        return 1;
    }

    string outputFileName = "../configuration/" + experiment
        + "/normalization/" + setToUse + ".root";
    TFile* outputFile = new TFile(outputFileName.c_str(), "RECREATE");

    for(auto& d : config.detectors)
    {
        if(!d.useForCS)
        {
            continue;
        }

        string polyHistoName = d.name + "TOF";
        TH1D* polyHisto = (TH1D*)polyFile->Get(polyHistoName.c_str());
        TH1D* polyMonitor = (TH1D*)polyFile->Get("CMONPSD");

        TH1D* blankRefHisto = (TH1D*)blankRefFile->Get(polyHistoName.c_str());
        TH1D* blankRefMonitor = (TH1D*)blankRefFile->Get("CMONPSD");

        TH1D* graphiteHisto = (TH1D*)graphiteFile->Get(polyHistoName.c_str());
        TH1D* graphiteMonitor = (TH1D*)graphiteFile->Get("CMONPSD");

        if(!polyHisto || !polyMonitor
                || !blankRefHisto || !blankRefMonitor
                || !graphiteHisto || !graphiteMonitor)
        {
            cerr << "Error: failed to open histogram needed for reference CS calculation." << endl;
            return 1;
        }

        double TOF = calculateTOF(d.distance, 0, 1.007627, reference.angle, config.neutronEnergy);

        subtractBackground(polyHisto, polyMonitor, graphiteHisto, graphiteMonitor, blankRefHisto, blankRefMonitor, TOF, reference, d);
    }

    outputFile->Close();

    return 0;
}
