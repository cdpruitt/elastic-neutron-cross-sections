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

void subtractBackground(TH1D* targetHist, TH1D* blankHist, TH1D* targetMonitor, TH1D* blankMonitor, string name, string distance)
{
    if(!targetHist || !blankHist)
    {
        cerr << "Error: empty histogram pointer passed to subtractBackground." << endl;
        return;
    }

    if(!targetMonitor || !blankMonitor)
    {
        cerr << "Error: passed scaling factor of 0 to subtractBackground." << endl;
        return;
    }

    string targetHistName = name + "_" + distance;
    string blankHistName = "blank_" + distance;

    TH1D* th = (TH1D*)targetHist->Clone(targetHistName.c_str());
    TH1D* bh = (TH1D*)blankHist->Clone(blankHistName.c_str());

    bh->Scale((double)targetMonitor->GetEntries()/blankMonitor->GetEntries());

    string differenceName = name + "Difference" + distance;
    TH1D* difference = (TH1D*)th->Clone(differenceName.c_str());
    difference->Add(bh,-1);

    difference->Write();
}

int main()
{
    AllConfigs allConfigs = getRunConfig();

    AllAngles allAngles;
    allAngles.getRunData(allConfigs);

    for(auto& angle : allAngles.angles)
    {
        for(auto& run : allConfigs.runs)
        {
            if(run.angle4M==angle.angle)
            {
                if(run.targetName=="Sn112")
                {
                    string histoFileName = "../analyzedData/runs/" + to_string(run.runNumber) + "/histos.root";
                    TFile histoFile(histoFileName.c_str(),"READ");

                    string Sn112HistoName = "4MTDC";
                    TH1D* Sn112Histo = (TH1D*)histoFile.Get(Sn112HistoName.c_str());
                    angle.Sn112Histo4M.push_back((TH1D*)(Sn112Histo->Clone()));
                    angle.Sn112Histo4M.back()->SetDirectory(0);

                    TH1D* Sn112Monitor = (TH1D*)histoFile.Get("CMONPSD");
                    angle.Sn112Monitor4M.push_back((TH1D*)(Sn112Monitor->Clone()));
                    angle.Sn112Monitor4M.back()->SetDirectory(0);

                    histoFile.Close();
                }

                else if(run.targetName=="Sn124")
                {
                    string histoFileName = "../analyzedData/runs/" + to_string(run.runNumber) + "/histos.root";
                    TFile histoFile(histoFileName.c_str(),"READ");

                    string Sn124HistoName = "4MTDC";
                    TH1D* Sn124Histo = (TH1D*)histoFile.Get(Sn124HistoName.c_str());
                    angle.Sn124Histo4M.push_back((TH1D*)(Sn124Histo->Clone()));
                    angle.Sn124Histo4M.back()->SetDirectory(0);

                    TH1D* Sn124Monitor = (TH1D*)histoFile.Get("CMONPSD");
                    angle.Sn124Monitor4M.push_back((TH1D*)(Sn124Monitor->Clone()));
                    angle.Sn124Monitor4M.back()->SetDirectory(0);

                    histoFile.Close();
                }

                else if(run.targetName=="blank")
                {
                    string histoFileName = "../analyzedData/runs/" + to_string(run.runNumber) + "/histos.root";
                    TFile histoFile(histoFileName.c_str(),"READ");

                    string blankHistoName = "4MTDC";
                    TH1D* blankHisto = (TH1D*)histoFile.Get(blankHistoName.c_str());
                    angle.blankHisto4M.push_back((TH1D*)(blankHisto->Clone()));
                    angle.blankHisto4M.back()->SetDirectory(0);

                    TH1D* blankMonitor = (TH1D*)histoFile.Get("CMONPSD");
                    angle.blankMonitor4M.push_back((TH1D*)(blankMonitor->Clone()));
                    angle.blankMonitor4M.back()->SetDirectory(0);

                    histoFile.Close();
                }
            }

            if(run.angle6M==angle.angle)
            {
                if(run.targetName=="Sn112")
                {
                    string histoFileName = "../analyzedData/runs/" + to_string(run.runNumber) + "/histos.root";
                    TFile histoFile(histoFileName.c_str(),"READ");

                    string Sn112HistoName = "6MTDC";
                    TH1D* Sn112Histo = (TH1D*)histoFile.Get(Sn112HistoName.c_str());
                    angle.Sn112Histo6M.push_back((TH1D*)(Sn112Histo->Clone()));
                    angle.Sn112Histo6M.back()->SetDirectory(0);

                    TH1D* Sn112Monitor = (TH1D*)histoFile.Get("CMONPSD");
                    angle.Sn112Monitor6M.push_back((TH1D*)(Sn112Monitor->Clone()));
                    angle.Sn112Monitor6M.back()->SetDirectory(0);

                    histoFile.Close();
                }

                else if(run.targetName=="Sn124")
                {
                    string histoFileName = "../analyzedData/runs/" + to_string(run.runNumber) + "/histos.root";
                    TFile histoFile(histoFileName.c_str(),"READ");

                    string Sn124HistoName = "6MTDC";
                    TH1D* Sn124Histo = (TH1D*)histoFile.Get(Sn124HistoName.c_str());
                    angle.Sn124Histo6M.push_back((TH1D*)(Sn124Histo->Clone()));
                    angle.Sn124Histo6M.back()->SetDirectory(0);

                    TH1D* Sn124Monitor = (TH1D*)histoFile.Get("CMONPSD");
                    angle.Sn124Monitor6M.push_back((TH1D*)(Sn124Monitor->Clone()));
                    angle.Sn124Monitor6M.back()->SetDirectory(0);

                    histoFile.Close();
                }

                else if(run.targetName=="blank")
                {
                    string histoFileName = "../analyzedData/runs/" + to_string(run.runNumber) + "/histos.root";
                    TFile histoFile(histoFileName.c_str(),"READ");

                    string blankHistoName = "6MTDC";
                    TH1D* blankHisto = (TH1D*)histoFile.Get(blankHistoName.c_str());
                    angle.blankHisto6M.push_back((TH1D*)(blankHisto->Clone()));
                    angle.blankHisto6M.back()->SetDirectory(0);

                    TH1D* blankMonitor = (TH1D*)histoFile.Get("CMONPSD");
                    angle.blankMonitor6M.push_back((TH1D*)(blankMonitor->Clone()));
                    angle.blankMonitor6M.back()->SetDirectory(0);

                    histoFile.Close();
                }
            }
        }

        // if directory for this group doesn't exist yet, create it
        struct stat st = {0};

        stringstream ss;
        ss << setprecision(4) << angle.angle;
        string dirName = "../analyzedData/angles/" + ss.str();
        if(stat(dirName.c_str(), &st) == -1)
        {
            mkdir(dirName.c_str(), 0700);
        }

        string outputFileName112= dirName + "/Sn112.root";
        TFile* outputFile112 = new TFile(outputFileName112.c_str(), "RECREATE");

        if(angle.Sn112Histo4M.size()>0)
        {
            TH1D* Sn112Histo4MTotal = (TH1D*)angle.Sn112Histo4M[0]->Clone("Sn112Histo4MTotal");
            Sn112Histo4MTotal->Reset();
            for(auto& histo : angle.Sn112Histo4M)
            {
                Sn112Histo4MTotal->Add(histo);
            }

            TH1D* Sn112Monitor4MTotal = (TH1D*)angle.Sn112Monitor4M[0]->Clone("Sn112Monitor4MTotal");
            Sn112Monitor4MTotal->Reset();
            for(auto& monitor : angle.Sn112Monitor4M)
            {
                Sn112Monitor4MTotal->Add(monitor);
            }

            TH1D* blankHisto4MTotal = (TH1D*)angle.blankHisto4M[0]->Clone("blankHisto4MTotal");
            blankHisto4MTotal->Reset();
            for(auto& histo : angle.blankHisto4M)
            {
                blankHisto4MTotal->Add(histo);
            }

            TH1D* blankMonitor4MTotal = (TH1D*)angle.blankMonitor4M[0]->Clone("blankMonitor4MTotal");
            blankMonitor4MTotal->Reset();
            for(auto& monitor : angle.blankMonitor4M)
            {
                blankMonitor4MTotal->Add(monitor);
            }

            subtractBackground(Sn112Histo4MTotal, blankHisto4MTotal, Sn112Monitor4MTotal, blankMonitor4MTotal, "Sn112", "4M");

            Sn112Histo4MTotal->Write();
            blankHisto4MTotal->Write();
            Sn112Monitor4MTotal->Write();
            blankMonitor4MTotal->Write();
        }

        if(angle.Sn112Histo6M.size()>0)
        {
            TH1D* Sn112Histo6MTotal = (TH1D*)angle.Sn112Histo6M[0]->Clone("Sn112Histo6MTotal");
            Sn112Histo6MTotal->Reset();
            for(auto& histo : angle.Sn112Histo6M)
            {
                Sn112Histo6MTotal->Add(histo);
            }

            TH1D* Sn112Monitor6MTotal = (TH1D*)angle.Sn112Monitor6M[0]->Clone("Sn112Monitor6MTotal");
            Sn112Monitor6MTotal->Reset();
            for(auto& monitor : angle.Sn112Monitor6M)
            {
                Sn112Monitor6MTotal->Add(monitor);
            }

            TH1D* blankHisto6MTotal = (TH1D*)angle.blankHisto6M[0]->Clone("blankHisto6MTotal");
            blankHisto6MTotal->Reset();
            for(auto& histo : angle.blankHisto6M)
            {
                blankHisto6MTotal->Add(histo);
            }

            TH1D* blankMonitor6MTotal = (TH1D*)angle.blankMonitor6M[0]->Clone("blankMonitor6MTotal");
            blankMonitor6MTotal->Reset();
            for(auto& monitor : angle.blankMonitor6M)
            {
                blankMonitor6MTotal->Add(monitor);
            }

            subtractBackground(Sn112Histo6MTotal, blankHisto6MTotal, Sn112Monitor6MTotal, blankMonitor6MTotal, "Sn112", "6M");

            Sn112Histo6MTotal->Write();
            blankHisto6MTotal->Write();
            Sn112Monitor6MTotal->Write();
            blankMonitor6MTotal->Write();
        }

        outputFile112->Close();

        string outputFileName124= dirName + "/Sn124.root";
        TFile* outputFile124 = new TFile(outputFileName124.c_str(), "RECREATE");

        if(angle.Sn124Histo4M.size()>0)
        {
            TH1D* Sn124Histo4MTotal = (TH1D*)angle.Sn124Histo4M[0]->Clone("Sn124Histo4MTotal");
            Sn124Histo4MTotal->Reset();
            for(auto& histo : angle.Sn124Histo4M)
            {
                Sn124Histo4MTotal->Add(histo);
            }

            TH1D* Sn124Monitor4MTotal = (TH1D*)angle.Sn124Monitor4M[0]->Clone("Sn124Monitor4MTotal");
            Sn124Monitor4MTotal->Reset();
            for(auto& monitor : angle.Sn124Monitor4M)
            {
                Sn124Monitor4MTotal->Add(monitor);
            }

            TH1D* blankHisto4MTotal = (TH1D*)angle.blankHisto4M[0]->Clone("blankHisto4MTotal");
            blankHisto4MTotal->Reset();
            for(auto& histo : angle.blankHisto4M)
            {
                blankHisto4MTotal->Add(histo);
            }

            TH1D* blankMonitor4MTotal = (TH1D*)angle.blankMonitor4M[0]->Clone("blankMonitor4MTotal");
            blankMonitor4MTotal->Reset();
            for(auto& monitor : angle.blankMonitor4M)
            {
                blankMonitor4MTotal->Add(monitor);
            }

            subtractBackground(Sn124Histo4MTotal, blankHisto4MTotal, Sn124Monitor4MTotal, blankMonitor4MTotal, "Sn124", "4M");

            Sn124Histo4MTotal->Write();
            blankHisto4MTotal->Write();
            Sn124Monitor4MTotal->Write();
            blankMonitor4MTotal->Write();
        }

        if(angle.Sn124Histo6M.size()>0)
        {
            TH1D* Sn124Histo6MTotal = (TH1D*)angle.Sn124Histo6M[0]->Clone("Sn124Histo6MTotal");
            Sn124Histo6MTotal->Reset();
            for(auto& histo : angle.Sn124Histo6M)
            {
                Sn124Histo6MTotal->Add(histo);
            }

            TH1D* Sn124Monitor6MTotal = (TH1D*)angle.Sn124Monitor6M[0]->Clone("Sn124Monitor6MTotal");
            Sn124Monitor6MTotal->Reset();
            for(auto& monitor : angle.Sn124Monitor6M)
            {
                Sn124Monitor6MTotal->Add(monitor);
            }

            TH1D* blankHisto6MTotal = (TH1D*)angle.blankHisto6M[0]->Clone("blankHisto6MTotal");
            blankHisto6MTotal->Reset();
            for(auto& histo : angle.blankHisto6M)
            {
                blankHisto6MTotal->Add(histo);
            }

            TH1D* blankMonitor6MTotal = (TH1D*)angle.blankMonitor6M[0]->Clone("blankMonitor6MTotal");
            blankMonitor6MTotal->Reset();
            for(auto& monitor : angle.blankMonitor6M)
            {
                blankMonitor6MTotal->Add(monitor);
            }

            subtractBackground(Sn124Histo6MTotal, blankHisto6MTotal, Sn124Monitor6MTotal, blankMonitor6MTotal, "Sn124", "6M");

            Sn124Histo6MTotal->Write();
            blankHisto6MTotal->Write();
            Sn124Monitor6MTotal->Write();
            blankMonitor6MTotal->Write();
        }

        outputFile124->Close();
    }

    return 0;
}
