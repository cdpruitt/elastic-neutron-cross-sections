#include "TH1D.h"
#include "TFile.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/ScalerData.h"
#include "../include/RunConfig.h"
#include "../include/DiffGroup.h"

using namespace std;

void subtractBackground(TH1D* targetHist, TH1D* blankHist, unsigned int targetScaling, unsigned int blankScaling, string name, string distance)
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

    string targetHistName = name + "_" + distance;
    string blankHistName = "blank_" + distance;

    TH1D* th = (TH1D*)targetHist->Clone(targetHistName.c_str());
    TH1D* bh = (TH1D*)blankHist->Clone(blankHistName.c_str());

    bh->Scale((double)targetScaling/blankScaling);

    string differenceName = name + "Difference" + "_" + distance;
    TH1D* difference = (TH1D*)th->Clone(differenceName.c_str());
    difference->Add(bh,-1);

    th->SetOption("l");
    bh->SetOption("l");

    th->Write();
    bh->Write();
    difference->Write();
}

int main()
{
    for(auto& group : readDiffHistoConfig())
    {
        // read target1 histo data
        string target1HistoFileName = "../analyzedData/runs/" + to_string(group.target1) + "/histos.root";
        TFile target1HistoFile(target1HistoFileName.c_str(),"READ");

        string target1_4MHistoName = "4MTDC";
        TH1D* target1_4MHisto = (TH1D*)target1HistoFile.Get(target1_4MHistoName.c_str());
        string target1_6MHistoName = "6MTDC";
        TH1D* target1_6MHisto = (TH1D*)target1HistoFile.Get(target1_6MHistoName.c_str());
        TH1D* target1MonitorHisto = (TH1D*)target1HistoFile.Get("CMONPSD");

        // read target2 histo data
        string target2HistoFileName = "../analyzedData/runs/" + to_string(group.target2) + "/histos.root";
        TFile target2HistoFile(target2HistoFileName.c_str(),"READ");

        string target2_4MHistoName = "4MTDC";
        TH1D* target2_4MHisto = (TH1D*)target2HistoFile.Get(target2_4MHistoName.c_str());
        string target2_6MHistoName = "6MTDC";
        TH1D* target2_6MHisto = (TH1D*)target2HistoFile.Get(target2_6MHistoName.c_str());
        TH1D* target2MonitorHisto = (TH1D*)target2HistoFile.Get("CMONPSD");

        // read blank histo data
        string blankHistoFileName = "../analyzedData/runs/" + to_string(group.blank) + "/histos.root";
        TFile blankHistoFile(blankHistoFileName.c_str(),"READ");

        string blank_4MHistoName = "4MTDC";
        TH1D* blank_4MHisto = (TH1D*)blankHistoFile.Get(blank_4MHistoName.c_str());
        string blank_6MHistoName = "6MTDC";
        TH1D* blank_6MHisto = (TH1D*)blankHistoFile.Get(blank_6MHistoName.c_str());
        TH1D* blankMonitorHisto = (TH1D*)blankHistoFile.Get("CMONPSD");

        // if directory for this group doesn't exist yet, create it
        string outputFileName = "../analyzedData/diffHistos/group" + to_string(group.groupNumber) + ".root";

        TFile* outputFile = new TFile(outputFileName.c_str(), "RECREATE");

        subtractBackground(target1_4MHisto, blank_4MHisto, target1MonitorHisto->GetEntries(), blankMonitorHisto->GetEntries(), "Sn112", "4M");
        subtractBackground(target2_4MHisto, blank_4MHisto, target2MonitorHisto->GetEntries(), blankMonitorHisto->GetEntries(), "Sn124", "4M");
        subtractBackground(target1_6MHisto, blank_6MHisto, target1MonitorHisto->GetEntries(), blankMonitorHisto->GetEntries(), "Sn112", "6M");
        subtractBackground(target2_6MHisto, blank_6MHisto, target2MonitorHisto->GetEntries(), blankMonitorHisto->GetEntries(), "Sn124", "6M");

        outputFile->Close();
    }

    return 0;
}
