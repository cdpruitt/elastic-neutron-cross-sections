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

#include "../include/Detector.h"
#include "../include/experimentalConstants.h"
#include "../include/histos.h"
#include "../include/Config.h"

extern Config config;

using namespace std;

void setBranches(TTree* t, vector<Detector>& detectors)
{
    for(auto& d : detectors)
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

    std::vector<Detector> detectors;
    for(auto& name : DETECTOR_NAMES)
    {
        detectors.push_back(Detector(name, config.experiment));
    }

    TFile outputFile(outputFileName.c_str(),"RECREATE");
    cout << "Creating histograms for " << inputFileName << endl;

    // connect tree to detector variables, so data can be read out from tree
    // into each detector
    setBranches(tree, detectors);

    // populate tree events into histos
    long treeEntries = tree->GetEntries();

    for(int i=0; i<treeEntries; i++)
    {
        tree->GetEvent(i);

        for(auto& d : detectors)
        {
            // skip events where another detector caused the trigger
            if(d.rawPH<=0)
            {
                continue;
            }

            // fill raw spectra before gates
            d.rawPH->Fill(d.pulseHeight);
            d.rawPSD->Fill(d.PSD);
            d.rawTDC->Fill(detectors[4].TDC);
            d.rawPHPSD->Fill(d.PSD, d.pulseHeight);

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
                    d.pulseHeightHisto->Fill(d.pulseHeight);
                    d.PSDHisto->Fill(d.PSD);
                    d.TDCHisto->Fill(detectors[4].TDC);
                    d.PHPSD->Fill(d.PSD, d.pulseHeight);
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

    for(auto& detector : detectors)
    {
        detector.write();
    }

    outputFile.Write();

    return 0;
}
