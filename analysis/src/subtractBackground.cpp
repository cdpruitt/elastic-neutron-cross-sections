#include "TH1I.h"
#include "TFile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>

#include "../include/ScalerData.h"
#include "../include/RunConfig.h"

using namespace std;

void subtractBackground(TH1I* targetHist, TH1I* blankHist, unsigned int targetScaling, unsigned int blankScaling, string outputFileName)
{
    if(!targetHist || !blankHist)
    {
        cerr << "Error: empty histogram pointer passed to subtractBackground." << endl;
        return;
    }

    if(targetScaling<=0 || blankScaling<=0)
    {
        cerr << "Error: passed scaling factor of 0 to subtractBackground." << endl;
        return;
    }

    TFile* outputFile = new TFile(outputFileName.c_str(), "RECREATE");

    TH1I* th = (TH1I*)targetHist->Clone("target");
    TH1I* bh = (TH1I*)blankHist->Clone("blank");

    bh->Scale((double)targetScaling/blankScaling);

    TH1I* difference = (TH1I*)th->Clone("difference");
    difference->Add(bh,-1);

    outputFile->cd();

    th->Write();
    bh->Write();
    difference->Write();

    outputFile->Close();
}

int main(int argc, char** argv)
{
    string detectorName = argv[1];

    vector<RunConfig> allConfigs = getRunConfig(detectorName);

    for(RunConfig rc1 : allConfigs)
    {
        // start by looking at runs with targets
        if(rc1.targetName=="blank" || rc1.targetName=="graphite" || rc1.targetName=="polyethylene")
        {
            continue;
        }

        for(RunConfig rc2 : allConfigs)
        {
            if(
                    (rc1.angle==
                     rc2.angle) &&
                    (rc2.targetName == "blank"))
            {
                // read beam flux information for target run
                string targetScalerFileName = "../rawData/runs/" + to_string(rc1.runNumber) + "/scalers.txt";
                ScalerData tsd(targetScalerFileName);

                string targetHistoFileName = "../analyzedData/runs/" + to_string(rc1.runNumber) + "/histos.root";
                TFile targetHistoFile(targetHistoFileName.c_str(),"READ");

                string targetHistoName = detectorName + "TDC";
                TH1I* targetHisto = (TH1I*)targetHistoFile.Get(targetHistoName.c_str());

                // read beam flux information for blank run
                string blankScalerFileName = "../rawData/runs/" + to_string(rc2.runNumber) + "/scalers.txt";
                ScalerData bsd(blankScalerFileName);

                string blankHistoFileName = "../analyzedData/runs/" + to_string(rc2.runNumber) + "/histos.root";
                TFile blankHistoFile(blankHistoFileName.c_str(),"READ");

                string blankHistoName = detectorName + "TDC";
                TH1I* blankHisto = (TH1I*)blankHistoFile.Get(blankHistoName.c_str());

                // if directory for this angle doesn't exist yet, create it
                string directoryName = "../analyzedData/angles/" + to_string((int)rc1.angle) + "/";

                struct stat st = {0};
                if(stat(directoryName.c_str(), &st) == -1)
                {
                    mkdir(directoryName.c_str(), 0700);
                }

                // produce output histo of difference between target and blank
                unsigned int runCounter = 0;
                string outputFileName = directoryName + rc1.targetName + "_" + to_string(runCounter) + ".root";

                while(ifstream(outputFileName))
                {
                    runCounter++;
                    outputFileName = directoryName + rc1.targetName + "_" + to_string(runCounter) + ".root";
                }

                if(!targetHisto || !blankHisto)
                {
                    continue;
                }

                subtractBackground(targetHisto, blankHisto, tsd.BCI, bsd.BCI, outputFileName);
            }
        }
    }

    return 0;
}
