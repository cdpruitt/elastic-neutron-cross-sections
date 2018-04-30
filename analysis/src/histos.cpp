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

    // read bananas for PID: 4M
    ifstream bananaFile4M("configuration/PSDGates/4MBanana.data");
    if(!bananaFile4M.is_open())
    {
        cerr << "Error: couldn't open banana file for 4M detector." << endl;
        exit(1);
    }

    vector<float> bananaXs4M;
    vector<float> bananaYs4M;

    string line;

    while(getline(bananaFile4M, line))
    {
        vector<string> tokens;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        bananaXs4M.push_back(stod(tokens[0]));
        bananaYs4M.push_back(stod(tokens[1]));
    }

    // read bananas for PID: 6M
    ifstream bananaFile6M("configuration/PSDGates/6MBanana.data");
    vector<float> bananaXs6M;
    vector<float> bananaYs6M;

    while(getline(bananaFile6M, line))
    {
        vector<string> tokens;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        bananaXs6M.push_back(stod(tokens[0]));
        bananaYs6M.push_back(stod(tokens[1]));
    }

    // read bananas for PID: monitor
    ifstream bananaFileMon("configuration/PSDGates/MonitorBanana.data");
    vector<float> bananaXsMon;
    vector<float> bananaYsMon;

    while(getline(bananaFileMon, line))
    {
        vector<string> tokens;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        bananaXsMon.push_back(stod(tokens[0]));
        bananaYsMon.push_back(stod(tokens[1]));
    }

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
            d.rawPHPSD->Fill(d.PSD, d.pulseHeight);

            // apply gates
            /*if(d.pulseHeight > d.pulseHeightThreshold)
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
            }*/

            // test that event is inside PSD-pulse height banana
            if(d.name=="4M")
            {
                if(TMath::IsInside(
                            d.PSD,
                            d.pulseHeight,
                            bananaXs4M.size(),
                            &bananaXs4M[0],
                            &bananaYs4M[0])
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

            else if(d.name=="6M")
            {
                if(TMath::IsInside(
                            d.PSD,
                            d.pulseHeight,
                            bananaYs6M.size(),
                            &bananaXs6M[0],
                            &bananaYs6M[0])
                  )

                    /*if(d.pulseHeight > d.pulseHeightThreshold &&
                      d.PSD > d.PSDThreshold)
                      */
                {
                    d.pulseHeightHisto->Fill(d.pulseHeight);
                    d.PSDHisto->Fill(d.PSD);
                    d.TDCHisto->Fill(detectors[4].TDC);
                    d.PHPSD->Fill(d.PSD, d.pulseHeight);
                }
            }

            else if(d.name=="CMON")
            {
                if(TMath::IsInside(
                            d.PSD,
                            d.pulseHeight,
                            bananaYsMon.size(),
                            &bananaXsMon[0],
                            &bananaYsMon[0])
                  )

                    /*if(d.pulseHeight > d.pulseHeightThreshold &&
                      d.PSD > d.PSDThreshold)
                      */
                {
                    d.pulseHeightHisto->Fill(d.pulseHeight);
                    d.PSDHisto->Fill(d.PSD);
                    d.TDCHisto->Fill(detectors[4].TDC);
                    d.PHPSD->Fill(d.PSD, d.pulseHeight);
                }

            }

            else
            {
                if(d.pulseHeight > d.pulseHeightThreshold &&
                      d.PSD > d.PSDThreshold)
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

    outputFile.Write();

    return 0;
}
