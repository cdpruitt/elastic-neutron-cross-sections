#include "TH1D.h"
#include "TFile.h"
#include "TArrow.h"
#include "TLine.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/Config.h"
#include "../include/experimentalConstants.h"
#include "../include/utilities.h"

extern Config config;

using namespace std;

void subtractBackground(
        vector<TH1D*> targetHistos,
        vector<TH1D*> targetMonitors,
        vector<TH1D*> blankHistos,
        vector<TH1D*> blankMonitors,
        string detector
        )
{
    if(targetHistos.size() > 0
            && targetMonitors.size() > 0
            && blankHistos.size() > 0
            && blankMonitors.size() > 0
      )
    {
        string histoName = "histo" + detector + "Total";
        TH1D* targetHistoTotal = (TH1D*)targetHistos[0]->Clone(histoName.c_str());
        targetHistoTotal->Reset();

        string monitorName = "monitor" + detector + "Total";
        TH1D* targetMonitorTotal = (TH1D*)targetMonitors[0]->Clone(monitorName.c_str());
        targetMonitorTotal->Reset();

        for(int i=0; i<targetHistos.size(); i++)
        {
            targetHistos[i]->Write();

            targetHistoTotal->Add(targetHistos[i]);
            targetMonitorTotal->Add(targetMonitors[i]);
        }

        targetHistoTotal->Write();
        targetMonitorTotal->Write();

        string blankName = "blank" + detector + "Total";
        TH1D* blankHistoTotal = (TH1D*)blankHistos[0]->Clone(blankName.c_str());
        blankHistoTotal->Reset();

        string blankMonitorName = "blankMonitor" + detector + "Total";
        TH1D* blankMonitorTotal = (TH1D*)blankMonitors[0]->Clone(blankMonitorName.c_str());
        blankMonitorTotal->Reset();

        for(int i=0; i<blankHistos.size(); i++)
        {
            blankHistos[i]->Write();

            blankHistoTotal->Add(blankHistos[i]);
            blankMonitorTotal->Add(blankMonitors[i]);
        }

        blankHistoTotal->Write();
        blankMonitorTotal->Write();
    }
}

int subtractBackground()
{
    cout << "Subtracting background..." << endl;

    for(auto& angle : config.angles)
    {
        for(auto& run : config.runs)
        {
            if(!run.isProduction)
            {
                continue;
            }

            for(auto& target : angle.targets)
            {
                if(target.name!=run.target.name)
                {
                    continue;
                }

                for(int i=0; i<run.angles.size(); i++)
                {
                    if(run.angles[i]==angle.value)
                    {
                        string histoFileName = "../processedData/" + config.experiment
                            + "/runs/" + to_string(run.number) + "/histos.root";
                        TFile histoFile(histoFileName.c_str(),"READ");

                        string histoName = config.detectors[i].name + "TOF_EC";
                        TH1D* histo = (TH1D*)histoFile.Get(histoName.c_str());
                        if(!histo)
                        {
                            cerr << "Error: couldn't open " << histoName << " in file "
                                << histoFileName << endl;
                            continue;
                        }

                        string newHistoName = "histo" + config.detectors[i].name + "_run" + to_string(run.number);
                        target.histos[i].push_back((TH1D*)(histo->Clone(newHistoName.c_str())));
                        target.histos[i].back()->SetDirectory(0);

                        TH1D* monitor = (TH1D*)histoFile.Get("CMONPSD");

                        string newMonitorName = "monitor_run" + to_string(run.number);
                        target.monitors[i].push_back((TH1D*)(monitor->Clone(newMonitorName.c_str())));
                        target.monitors[i].back()->SetDirectory(0);

                        histoFile.Close();
                    }
                }
            }
        }

        // if directory for this angle doesn't exist yet, create it
        struct stat st = {0};

        stringstream ss;
        ss << setprecision(5) << angle.value;
        string dirName = "../processedData/" + config.experiment + "/angles/" + ss.str();
        if(stat(dirName.c_str(), &st) == -1)
        {
            mkdir(dirName.c_str(), 0700);
        }

        for(auto& target : angle.targets)
        {
            string outputFileName = dirName + "/" + target.name + ".root";
            TFile* outputFile = new TFile(outputFileName.c_str(), "RECREATE");

            for(int i=0; i<target.histos.size(); i++)
            {
                subtractBackground(target.histos[i], target.monitors[i], angle.getBlank().histos[i], angle.getBlank().monitors[i], config.detectors[i].name);
            }

            outputFile->Close();
        }
    }

    return 0;
}
