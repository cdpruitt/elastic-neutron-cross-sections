#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1.h"

#include "../include/Detector.h"
#include "../include/experimentalConstants.h"

using namespace std;

void setBranches(TTree* t, vector<Detector>& detectors)
{
    for(Detector& d : detectors)
    {
        string PHName = d.name + "PH";
        string PSDName = d.name + "PSD";
        string TDCName = d.name + "TDC";

        t->SetBranchAddress(PHName.c_str(),&d.pulseHeight);
        t->SetBranchAddress(PSDName.c_str(),&d.PSD);
        t->SetBranchAddress(TDCName.c_str(),&d.TDC);
    }
}

int main(int argc, char** argv)
{
    string inputFileName = argv[1];
    string outputFileName = argv[2];

    TFile inputFile(inputFileName.c_str());
    TTree* tree = (TTree*)inputFile.Get("tree");
    if(!tree)
    {
        cerr << "Error: failed to open tree in " << inputFileName << ". Exiting..." << endl;
        exit(1);
    }

    cout << "Creating histograms for " << inputFileName << endl;

    TFile outputFile(outputFileName.c_str(),"RECREATE");

    // initialize the detectors used for the experiment
    vector<Detector> detectors;
    for(string s : DETECTOR_NAMES)
    {
        detectors.push_back(Detector(s));
    }

    // connect tree to detector variables, so data can be read out from tree
    // into each detector
    setBranches(tree, detectors);

    // populate tree events into histos
    long treeEntries = tree->GetEntries();

    for(int i=0; i<treeEntries; i++)
    {
        tree->GetEvent(i);

        for(Detector d : detectors)
        {
            // fill raw spectra before gates
            d.rawPH->Fill(d.pulseHeight);
            d.rawPSD->Fill(d.PSD);
            d.rawTDC->Fill(detectors[4].TDC);

            // apply gates
            if(d.pulseHeight > d.pulseHeightThreshold)
            {
                d.PHGatedPH->Fill(d.pulseHeight);
                d.PHGatedPSD->Fill(d.PSD);
                d.PHGatedTDC->Fill(detectors[4].TDC);
            }

            if(d.PSD > d.PSDThreshold)
            {
                d.PSDGatedPH->Fill(d.pulseHeight);
                d.PSDGatedPSD->Fill(d.PSD);
                d.PSDGatedTDC->Fill(detectors[4].TDC);
            }

            if(d.pulseHeight > d.pulseHeightThreshold &&
                    d.PSD > d.PSDThreshold)
            {
                d.pulseHeightHisto->Fill(d.pulseHeight);
                d.PSDHisto->Fill(d.PSD);
                d.TDCHisto->Fill(detectors[4].TDC);
            }
        }

        // print progress every 10000 events
        if (i%10000 == 0)
        {
            cout << "Processed " << i << " events\r";
            fflush(stdout);
        }
    }

    outputFile.Write();

    return 0;
}
