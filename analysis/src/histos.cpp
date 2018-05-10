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

extern Config config;

using namespace std;

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
        detector.createHistos();
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
    }

    outputFile.Write();

    return 0;
}
