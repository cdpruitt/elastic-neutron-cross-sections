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

using namespace std;

void subtractBackground(
        TH1D* polyHisto,
        TH1D* polyMon,
        TH1D* graphiteHisto,
        TH1D* graphiteMon,
        TH1D* blankRefHisto,
        TH1D* blankRefMon,
        double lowIntegrationLimit,
        double highIntegrationLimit,
        ReferenceCS& reference,
        string detector
        )
{
    //int minIntBin = polyMon->FindBin(intLimits.low[2]);
    //int maxIntBin = polyMon->FindBin(intLimits.high[2]);

    //double polyMonCounts = polyMon->Integral(minIntBin, maxIntBin);
    //double graphiteMonCounts = graphiteMon->Integral(minIntBin, maxIntBin);
    //double blankRefMonCounts = blankRefMon->Integral(minIntBin, maxIntBin);

    double polyMonCounts = polyMon->GetEntries();
    double graphiteMonCounts = graphiteMon->GetEntries();
    double blankRefMonCounts = blankRefMon->GetEntries();

    polyHisto->Scale(NORMALIZATION_SCALING/polyMonCounts);
    polyHisto->Write();
    graphiteHisto->Scale(NORMALIZATION_SCALING/graphiteMonCounts);
    graphiteHisto->Write();
    blankRefHisto->Scale(NORMALIZATION_SCALING/blankRefMonCounts);
    blankRefHisto->Write();

    string polyMinusBlankName = "polyMinusBlank" + detector;
    TH1D* polyMinusBlank = (TH1D*)polyHisto->Clone(polyMinusBlankName.c_str());
    polyMinusBlank->Add(blankRefHisto, -1);
    polyMinusBlank->Write();

    string graphiteMinusBlankName = "graphiteMinusBlank" + detector;
    TH1D* graphiteMinusBlank = (TH1D*)graphiteHisto->Clone(graphiteMinusBlankName.c_str());
    graphiteMinusBlank->Add(blankRefHisto, -1);
    graphiteMinusBlank->Write();

    string diffHistoName = "polyMinusGraphite" + detector;
    TH1D* diffHisto = (TH1D*)polyMinusBlank->Clone(diffHistoName.c_str());
    //double molRatio = reference.polyNumberOfAtoms/reference.graphiteNumberOfAtoms;
    //graphiteMinusBlank->Scale(molRatio);
    diffHisto->Add(graphiteMinusBlank, -1);
    diffHisto->Write();

    // perform integrals
    int minIntBin = diffHisto->FindBin(lowIntegrationLimit);
    int maxIntBin = diffHisto->FindBin(highIntegrationLimit);
    reference.counts.push_back(diffHisto->Integral(minIntBin, maxIntBin));
    reference.monitors.push_back(polyMonCounts);
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
    IntegrationLimits intLimits;

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

        else if(tokens[0] == "Low")
        {
            intLimits.low.push_back(stod(tokens[3]));
        }

        else if(tokens[0] == "High")
        {
            intLimits.high.push_back(stod(tokens[3]));
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

    TH1D* polyHisto4M = (TH1D*)polyFile->Get("4MTDC");
    TH1D* polyHisto6M = (TH1D*)polyFile->Get("6MTDC");
    TH1D* polyMonitor = (TH1D*)polyFile->Get("CMONPSD");

    TH1D* blankRefHisto4M = (TH1D*)blankRefFile->Get("4MTDC");
    TH1D* blankRefHisto6M = (TH1D*)blankRefFile->Get("6MTDC");
    TH1D* blankRefMonitor = (TH1D*)blankRefFile->Get("CMONPSD");

    TH1D* graphiteHisto4M = (TH1D*)graphiteFile->Get("4MTDC");
    TH1D* graphiteHisto6M = (TH1D*)graphiteFile->Get("6MTDC");
    TH1D* graphiteMonitor = (TH1D*)graphiteFile->Get("CMONPSD");

    if(!polyHisto4M || !polyHisto6M || !polyMonitor
            || !blankRefHisto4M || !blankRefHisto6M || !blankRefMonitor
            || !graphiteHisto4M || !graphiteHisto6M || !graphiteMonitor)
    {
        cerr << "Error: failed to open histogram needed for reference CS calculation." << endl;
        return 1;
    }

    string outputFileName = "../configuration/" + experiment
        + "/normalization/" + setToUse + ".root";
    TFile* outputFile = new TFile(outputFileName.c_str(), "RECREATE");

    subtractBackground(polyHisto4M, polyMonitor, graphiteHisto4M, graphiteMonitor, blankRefHisto4M, blankRefMonitor, intLimits.low[0], intLimits.high[0], reference, "4M");
    subtractBackground(polyHisto6M, polyMonitor, graphiteHisto6M, graphiteMonitor, blankRefHisto6M, blankRefMonitor, intLimits.low[1], intLimits.high[1], reference, "6M");

    outputFile->Close();

    return 0;
}
