#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"

#include "../include/DetectorHistos.h"
#include "../include/histos.h"
#include "../include/Config.h"
#include "../include/physicalConstants.h"

extern Config config;

using namespace std;

TH1D* correctForEfficiency(TH1D* histo, const Detector& d)
{
    string name = histo->GetName();
    name += "_EC";

    TH1D* histoEC = (TH1D*)histo->Clone(name.c_str());

    for(int i=1; i<histoEC->GetNbinsX(); i++)
    {
        double TOF = i*d.linearCalibration;
        double velocity = ((d.distance/TOF)*pow(10,7))/C;
        if(velocity>1 || velocity <0)
        {
            continue;
        }

        double energy = 0.5*NEUTRON_MASS*AMU_TO_MEVC2*pow(velocity,2);
        double efficiency = d.efficiency.getEfficiency(energy);

        if(efficiency <= 0 || efficiency>1)
        {
            continue;
        }

        double binValue = histoEC->GetBinContent(i)/efficiency;

        if((binValue < 0) || (binValue!=binValue) || (std::isinf(binValue)))
        {
            cerr << "Error: value for bin " << i << " was " << binValue
                << " (energy = " << energy << ", efficiency = " << efficiency
                << ")" << endl;
            exit(1);
        }

        histoEC->SetBinContent(i, histoEC->GetBinContent(i)/efficiency);
    }

    return histoEC;
}

void setBranches(TTree* t)
{
    for(auto& d : config.detectors)
    {
        string PHName = d.name + "PH";
        string PSDName = d.name + "PSD";
        string TDCName = d.name + "TDC";

        t->SetBranchAddress(PHName.c_str(),&d.pulseHeight);
        t->SetBranchAddress(PSDName.c_str(),&d.PSD);
        t->SetBranchAddress(TDCName.c_str(),&d.TDC);
    }
}

int histos(string inputFileName, string outputFileName)
{
    TFile inputFile(inputFileName.c_str(), "READ");
    TTree* tree = (TTree*)inputFile.Get("tree");
    if(!tree)
    {
        cerr << "Error: failed to open tree in " << inputFileName << ". Exiting..." << endl;
        exit(1);
    }

    TFile outputFile(outputFileName.c_str(),"RECREATE");
    cout << "Creating histograms for " << inputFileName << endl;

    // connect tree to detector variables, so data can be read out from tree
    // into each detector
    setBranches(tree);

    // create new histograms for recording this run
    for(auto& detector : config.detectors)
    {
        detector.histos = DetectorHistos(detector.name, detector.linearCalibration);
    }

    // populate tree events into histos
    long treeEntries = tree->GetEntries();

    for(int i=0; i<treeEntries; i++)
    {
        tree->GetEvent(i);

        for(auto& d : config.detectors)
        {
            // skip events where another detector caused the trigger
            if(d.histos.rawPH<=0)
            {
                continue;
            }

            // fill raw spectra before gates
            d.histos.rawPH->Fill(d.pulseHeight);
            d.histos.rawPSD->Fill(d.PSD);
            d.histos.rawTDC->Fill(config.detectors[4].TDC);
            d.histos.rawPHPSD->Fill(d.PSD, d.pulseHeight);

            // test that event is inside PSD-pulse height banana
            if(d.name=="4M" || d.name=="6M" || d.name=="CMON")
            {
                if(TMath::IsInside(
                            d.PSD,
                            d.pulseHeight,
                            d.banana.X.size(),
                            &d.banana.X[0],
                            &d.banana.Y[0])
                  )

                  /*if(d.pulseHeight > d.pulseHeightThreshold &&
                      d.PSD > d.PSDThreshold)*/
                {
                    d.histos.pulseHeightHisto->Fill(d.pulseHeight);
                    d.histos.PSDHisto->Fill(d.PSD);
                    d.histos.TDCHisto->Fill(config.detectors[4].TDC);
                    d.histos.PHPSD->Fill(d.PSD, d.pulseHeight);

                    double neutronTOF =
                        ((d.linearCalibration*d.TDC+d.timeOffset)-
                        (config.detectors[4].linearCalibration
                         *config.detectors[4].TDC
                         +config.detectors[4].timeOffset));

                    /*cout << "detector " << d.name << " TDC = " << d.TDC
                        << ", PO TDC = " << config.detectors[4].TDC
                        << ", neutronTOF = " << neutronTOF << endl;
                        */

                    d.histos.TOFHisto->Fill(neutronTOF);
                }
            }
        }

        // print progress every 10000 events
        if (i%10000 == 0)
        {
            cout << "Processed " << i << " events\r";
            fflush(stdout);
        }
    }

    for(auto& detector : config.detectors)
    {
        detector.histos.write();

        if(detector.useForCS)
        {
            TH1D* TOFHistoEC = correctForEfficiency(detector.histos.TOFHisto, detector);
            TOFHistoEC->Write();
        }
    }

    return 0;
}
