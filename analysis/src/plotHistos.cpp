#include "../include/Config.h"
#include "../include/utilities.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLine.h"
#include "TImage.h"
#include "TFile.h"

#include <sstream>

using namespace std;

extern Config config;

int plotHistos()
{
    const unsigned int CANVAS_WIDTH = 3;
    const unsigned int CANVAS_HEIGHT = 4;

    for(int i=0; i<config.runs.size(); i++)
    {
        TCanvas* c;

        if(i%CANVAS_HEIGHT==0)
        {
            c = new TCanvas(to_string(config.runs[i].number).c_str(),"",CANVAS_WIDTH*280,CANVAS_HEIGHT*200);
            c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);
        }

        for(int detector=0; detector<config.detectors.size(); detector++)
        {
            string fileName = "../processedData/" + config.experiment + "/runs/" + to_string(config.runs[i].number) + "/histos.root";
            TFile* file = new TFile(fileName.c_str(),"READ");

            if(!file->IsOpen())
            {
                cerr << "Error: couldn't open " << fileName << endl;
                return 1;
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

                string TDCName = config.detectors[detector].name + "TDC";
                TH1I* TDC = (TH1I*)file->Get(TDCName.c_str());

                if(TDC)
                {
                    // Pad dimensions and margins
                    //gPad->SetPad(0, 0.8, 1, 0);
                    gPad->SetLeftMargin(0.15);
                    gPad->SetRightMargin(0.0);
                    gPad->SetTopMargin(0.15);
                    gPad->SetBottomMargin(0.18);
                    //gPad->SetTicky(2);

                    // Set histo point and line characteristics
                    TDC->SetLineColor(kBlack);
                    TDC->SetLineWidth(2);
                    TDC->SetLineStyle(0);

                    // X-axis parameters
                    TDC->GetXaxis()->SetTitle("TOF (a.u.)");
                    TDC->GetXaxis()->SetTitleSize(0.06);
                    TDC->GetXaxis()->SetTitleFont(2);
                    TDC->GetXaxis()->SetTitleOffset(1.4);
                    TDC->GetXaxis()->CenterTitle();

                    TDC->GetXaxis()->SetLabelOffset(0.01);
                    TDC->GetXaxis()->SetLabelSize(0.05);
                    TDC->GetXaxis()->SetLabelFont(2);

                    TDC->GetXaxis()->SetNdivisions(10);
                    TDC->GetXaxis()->SetTickLength(0.03);

                    // Y-axis parameters
                    TDC->GetYaxis()->SetTitle("Counts");
                    TDC->GetYaxis()->SetTitleSize(0.06);
                    TDC->GetYaxis()->SetTitleFont(2);
                    TDC->GetYaxis()->SetTitleOffset(1.3);
                    TDC->GetYaxis()->CenterTitle();

                    TDC->GetYaxis()->SetLabelOffset(0.01);
                    TDC->GetYaxis()->SetLabelSize(0.05);

                    TDC->GetYaxis()->SetLabelFont(2);
                    TDC->GetYaxis()->SetNdivisions(10);
                    TDC->GetYaxis()->SetTickLength(0);

                    string title = "Run " + to_string(config.runs[i].number) + ", " + config.detectors[detector].name + " TDC";
                    TDC->SetTitle(title.c_str());

                    TDC->Draw("hist");

                    double TOF = calculateTOF(config.detectors[detector].distance, config.runs[i].target.getMolarMass(), angle, config.neutronEnergy);

                    cout << "TOF = " << TOF << endl;

                    TDC->GetXaxis()->SetRangeUser(TOF-100, TOF+100);

                    int minIntBin = TDC->FindBin(TOF-1);
                    int maxIntBin = TDC->FindBin(TOF+1);

                    ostringstream ss;

                    double yAxisLowEdge = TDC->GetMinimum();
                    double yAxisHighEdge = TDC->GetMaximum();

                    TLine *gateLowLine = new TLine(TOF-1,yAxisLowEdge,TOF-1,yAxisHighEdge);
                    TLine *gateHighLine = new TLine(TOF+1,yAxisLowEdge,TOF+1,yAxisHighEdge);
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

            string name = "../processedData/" + config.experiment + "/runs/" + to_string(i/CANVAS_HEIGHT) + ".png";
            img->WriteImage(name.c_str());

            delete c;
        }
    }

    return 0;
}
