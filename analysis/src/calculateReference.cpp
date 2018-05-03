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
#include "../include/experimentalConstants.h"

using namespace std;

void subtractBackground(
        vector<TH1D*> targetHistos,
        vector<TH1D*> targetMonitors,
        vector<TH1D*> blankHistos,
        vector<TH1D*> blankMonitors,
        string targetName,
        string detectorName
        )
{
    if(targetHistos.size() > 0
            && targetMonitors.size() > 0
            && blankHistos.size() > 0
            && blankMonitors.size() > 0
      )
    {
        string histoName = "histo" + detectorName + "Total";
        TH1D* targetHistoTotal = (TH1D*)targetHistos[0]->Clone(histoName.c_str());
        targetHistoTotal->Reset();

        for(int i=0; i<targetHistos.size(); i++)
        {
            targetHistoTotal->Add(
                    targetHistos[i],
                    NORMALIZATION_SCALING/(double)targetMonitors[i]->GetEntries()
                    );

            targetHistos[i]->Write();
        }

        targetHistoTotal->Write();

        string blankName = "blank" + detectorName + "Total";
        TH1D* blankHistoTotal = (TH1D*)blankHistos[0]->Clone(blankName.c_str());
        blankHistoTotal->Reset();

        for(int i=0; i<blankHistos.size(); i++)
        {
            blankHistoTotal->Add(
                    blankHistos[i],
                    (targetHistos.size()/(double)blankHistos.size())*
                    NORMALIZATION_SCALING/(double)blankMonitors[i]->GetEntries()
                    );

            blankHistos[i]->Write();
        }

        blankHistoTotal->Write();

        string diffName = "diff" + detectorName;
        TH1D* diffHisto = (TH1D*)targetHistoTotal->Clone(diffName.c_str());

        diffHisto->Add(blankHistoTotal, -1);
        diffHisto->Write();
    }
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
                for(auto& target : angle.targets)
                {
                    if(run.targetName==target.name)
                    {
                        string histoFileName = "../processedData/runs/" + to_string(run.runNumber) + "/histos.root";
                        TFile histoFile(histoFileName.c_str(),"READ");

                        string histoName = "4MTDC";
                        TH1D* histo = (TH1D*)histoFile.Get(histoName.c_str());

                        string newHistoName = "histo4M_run" + to_string(run.runNumber);
                        target.histo4M.push_back((TH1D*)(histo->Clone(newHistoName.c_str())));
                        target.histo4M.back()->SetDirectory(0);

                        TH1D* monitor = (TH1D*)histoFile.Get("CMONPSD");

                        string newMonitorName = "monitor_run" + to_string(run.runNumber);
                        target.monitor4M.push_back((TH1D*)(monitor->Clone(newMonitorName.c_str())));
                        target.monitor4M.back()->SetDirectory(0);

                        histoFile.Close();
                    }
                }
            }

            if(run.angle6M==angle.angle)
            {
                for(auto& target : angle.targets)
                {
                    if(run.targetName==target.name)
                    {
                        string histoFileName = "../processedData/runs/" + to_string(run.runNumber) + "/histos.root";
                        TFile histoFile(histoFileName.c_str(),"READ");

                        string histoName = "6MTDC";
                        TH1D* histo = (TH1D*)histoFile.Get(histoName.c_str());

                        string newHistoName = "histo6M_run" + to_string(run.runNumber);
                        target.histo6M.push_back((TH1D*)(histo->Clone(newHistoName.c_str())));
                        target.histo6M.back()->SetDirectory(0);

                        TH1D* monitor = (TH1D*)histoFile.Get("CMONPSD");

                        string newMonitorName = "monitor_run" + to_string(run.runNumber);
                        target.monitor6M.push_back((TH1D*)(monitor->Clone(newMonitorName.c_str())));
                        target.monitor6M.back()->SetDirectory(0);

                        histoFile.Close();
                    }
                }
            }
        }

        // if directory for this group doesn't exist yet, create it
        struct stat st = {0};

        stringstream ss;
        ss << setprecision(5) << angle.angle;
        string dirName = "../processedData/angles/" + ss.str();
        if(stat(dirName.c_str(), &st) == -1)
        {
            mkdir(dirName.c_str(), 0700);
        }

        for(auto& target : angle.targets)
        {
            string outputFileName = dirName + "/" + target.name + ".root";
            TFile* outputFile = new TFile(outputFileName.c_str(), "RECREATE");

            subtractBackground(target.histo4M, target.monitor4M, angle.getBlank().histo4M, angle.getBlank().monitor4M, target.name, "4M");
            subtractBackground(target.histo6M, target.monitor6M, angle.getBlank().histo6M, angle.getBlank().monitor6M, target.name, "6M");

            outputFile->Close();
        }
    }

    return 0;
}
