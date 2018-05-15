#include "../include/Config.h"
#include "../include/utilities.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLine.h"
#include "TImage.h"
#include "TFile.h"
#include "TArrow.h"

#include <sstream>

using namespace std;

extern Config config;

int plotHistos()
{
    const unsigned int CANVAS_WIDTH = 2;
    const unsigned int CANVAS_HEIGHT = 4;

    for(int i=0; i<config.runs.size(); i++)
    {
        auto& t = config.runs[i].target;

        TCanvas* c;

        if(i%CANVAS_HEIGHT==0)
        {
            c = new TCanvas(to_string(config.runs[i].number).c_str(),"",CANVAS_WIDTH*380,CANVAS_HEIGHT*200);
            c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);
        }

        string fileName = "../processedData/" + config.experiment + "/runs/" + to_string(config.runs[i].number) + "/histos.root";
        TFile* file = new TFile(fileName.c_str(),"READ");

        if(!file->IsOpen())
        {
            cerr << "Error: couldn't open " << fileName << endl;
            return 1;
        }

        for(int detector=0; detector<config.detectors.size(); detector++)
        {
            auto& d = config.detectors[detector];

            if(!d.useForCS)
            {
                continue;
            }

            {
                c->cd(detector+1+CANVAS_WIDTH*(i%CANVAS_HEIGHT));
                double angle;

                if(detector==0)
                {
                    angle = config.runs[i].angles[0];
                }

                else if(detector==1)
                {
                    angle = config.runs[i].angles[1];
                }

                else
                {
                    angle = 0;
                }

                string TOFName = d.name + "TOF";
                TH1I* TOFHisto = (TH1I*)file->Get(TOFName.c_str());

                if(TOFHisto)
                {
                    // Pad dimensions and margins
                    //gPad->SetPad(0, 0.8, 1, 0);
                    gPad->SetLeftMargin(0.15);
                    gPad->SetRightMargin(0.0);
                    gPad->SetTopMargin(0.15);
                    gPad->SetBottomMargin(0.18);
                    //gPad->SetTicky(2);

                    // Set histo point and line characteristics
                    TOFHisto->SetLineColor(kBlack);
                    TOFHisto->SetLineWidth(2);
                    TOFHisto->SetLineStyle(0);

                    // X-axis parameters
                    TOFHisto->GetXaxis()->SetTitle("TOF (ns)");
                    TOFHisto->GetXaxis()->SetTitleSize(0.06);
                    TOFHisto->GetXaxis()->SetTitleFont(2);
                    TOFHisto->GetXaxis()->SetTitleOffset(1.4);
                    TOFHisto->GetXaxis()->CenterTitle();

                    TOFHisto->GetXaxis()->SetLabelOffset(0.01);
                    TOFHisto->GetXaxis()->SetLabelSize(0.05);
                    TOFHisto->GetXaxis()->SetLabelFont(2);

                    TOFHisto->GetXaxis()->SetNdivisions(10);
                    TOFHisto->GetXaxis()->SetTickLength(0.03);

                    // Y-axis parameters
                    TOFHisto->GetYaxis()->SetTitle("Counts");
                    TOFHisto->GetYaxis()->SetTitleSize(0.06);
                    TOFHisto->GetYaxis()->SetTitleFont(2);
                    TOFHisto->GetYaxis()->SetTitleOffset(1.3);
                    TOFHisto->GetYaxis()->CenterTitle();

                    TOFHisto->GetYaxis()->SetLabelOffset(0.01);
                    TOFHisto->GetYaxis()->SetLabelSize(0.05);

                    TOFHisto->GetYaxis()->SetLabelFont(2);
                    TOFHisto->GetYaxis()->SetNdivisions(10);
                    TOFHisto->GetYaxis()->SetTickLength(0);

                    string title = "Run " + to_string(config.runs[i].number) + " (" + t.name + "), " + d.name + " TOF";
                    TOFHisto->SetTitle(title.c_str());

                    TOFHisto->Draw("hist");

                    double TOF = calculateTOF(d.distance, 0, t.getMolarMass(), angle, config.neutronEnergy);

                    TOFHisto->GetXaxis()->SetRangeUser(TOF-10, TOF+20);

                    double TOFResolution = d.resolution*d.linearCalibration; // FWHM, in ns
                    int minIntBin = TOFHisto->FindBin(TOF-TOFResolution);
                    int maxIntBin = TOFHisto->FindBin(TOF+TOFResolution);

                    double yAxisLowEdge = TOFHisto->GetMinimum();
                    double yAxisHighEdge = TOFHisto->GetMaximum();

                    for(int state=0; state<t.excitedStates.size(); state++)
                    {
                        double excitedTOF = calculateTOF(d.distance, t.excitedStates[state], t.getMolarMass(), angle, config.neutronEnergy);

                        double numberOfCounts = TOFHisto->GetBinContent(TOFHisto->GetBin(excitedTOF));
                        TArrow *arrow = new TArrow(excitedTOF, yAxisHighEdge, excitedTOF, numberOfCounts, 0.015, "|>");
                        arrow->SetAngle(30);
                        arrow->SetLineWidth(3);
                        arrow->SetLineColor(kRed+2);
                        arrow->SetFillColor(kRed+2);
                        arrow->Draw();
                    }

                    double nitrogenTOF = calculateTOF(d.distance, 0, 14.001, angle, config.neutronEnergy);
                    double numberOfCounts = TOFHisto->GetBinContent(TOFHisto->GetBin(nitrogenTOF));
                    TArrow *arrow = new TArrow(nitrogenTOF, yAxisHighEdge, nitrogenTOF, numberOfCounts, 0.015, "|>");
                    arrow->SetAngle(30);
                    arrow->SetLineWidth(3);
                    arrow->SetLineColor(kGreen+2);
                    arrow->SetFillColor(kGreen+2);
                    arrow->Draw();

                    TLine *gateLowLine = new TLine(TOF-TOFResolution,yAxisLowEdge,TOF-TOFResolution,yAxisHighEdge);
                    TLine *gateHighLine = new TLine(TOF+TOFResolution,yAxisLowEdge,TOF+TOFResolution,yAxisHighEdge);
                    gateLowLine->SetLineStyle(7);
                    gateLowLine->SetLineWidth(3);
                    gateLowLine->SetLineColor(kBlue);
                    gateLowLine->Draw();

                    gateHighLine->SetLineStyle(7);
                    gateHighLine->SetLineWidth(3);
                    gateHighLine->SetLineColor(kBlue);
                    gateHighLine->Draw();

                    
                }
            }
        }

        if(i%CANVAS_HEIGHT==(CANVAS_HEIGHT-1))
        {
            c->Update();
            TImage *img = TImage::Create();

            img->FromPad(c);

            string name = "../processedData/" + config.experiment + "/plots/" + to_string(i/CANVAS_HEIGHT) + ".png";
            img->WriteImage(name.c_str());

            delete c;
        }
    }

    return 0;
}
