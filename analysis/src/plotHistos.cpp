#include "../include/Config.h"
#include "../include/utilities.h"
#include "../include/experimentalConstants.h"
#include "../include/ReferenceCS.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLine.h"
#include "TImage.h"
#include "TFile.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TStyle.h"

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
                    double TOFResolution = d.resolution*d.linearCalibration; // FWHM, in ns

                    double yAxisLowEdge = TOFHisto->GetMinimum();
                    double yAxisHighEdge = TOFHisto->GetMaximum();

                    double maxExcitedTOF = 0;
                    for(int state=0; state<t.excitedStates.size(); state++)
                    {
                        double excitedTOF = calculateTOF(d.distance, t.excitedStates[state], t.getMolarMass(), angle, config.neutronEnergy);

                        if(maxExcitedTOF<excitedTOF)
                        {
                            maxExcitedTOF = excitedTOF;
                        }

                        double numberOfCounts = TOFHisto->GetBinContent(TOFHisto->GetBin(excitedTOF));
                        TArrow *arrow = new TArrow(excitedTOF, yAxisHighEdge, excitedTOF, numberOfCounts, 0.015, "|>");
                        arrow->SetAngle(30);
                        arrow->SetLineWidth(3);
                        arrow->SetLineColor(kRed+2);
                        arrow->SetFillColor(kRed+2);
                        arrow->Draw();
                    }

                    double nitrogenTOF = calculateTOF(d.distance, 0, 14.001, angle, config.neutronEnergy);
                    double nitrogenCounts = TOFHisto->GetBinContent(TOFHisto->GetBin(nitrogenTOF));
                    TArrow *arrow = new TArrow(nitrogenTOF, yAxisHighEdge, nitrogenTOF, nitrogenCounts, 0.015, "|>");
                    arrow->SetAngle(30);
                    arrow->SetLineWidth(3);
                    arrow->SetLineColor(kGreen+2);
                    arrow->SetFillColor(kGreen+2);
                    arrow->Draw();

                    double counts = TOFHisto->GetBinContent(TOFHisto->GetBin(TOF));
                    TArrow *elasticArrow = new TArrow(TOF, yAxisHighEdge, TOF, counts, 0.015, "|>");
                    elasticArrow->SetAngle(30);
                    elasticArrow->SetLineWidth(3);
                    elasticArrow->SetLineColor(kBlue+2);
                    elasticArrow->SetFillColor(kBlue+2);
                    elasticArrow->Draw();

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

                    double maxTOFRange = max(nitrogenTOF, maxExcitedTOF);
                    TOFHisto->GetXaxis()->SetRangeUser(TOF-10, maxTOFRange+10);
                }
            }
        }

        if(i%CANVAS_HEIGHT==(CANVAS_HEIGHT-1))
        {
            c->Update();
            TImage *img = TImage::Create();

            img->FromPad(c);

            string name = "../processedData/" + config.experiment + "/plots/set" + to_string(i/CANVAS_HEIGHT) + ".png";
            img->WriteImage(name.c_str());

            delete c;
        }
    }

    return 0;
}

int plotDiffs()
{
    const unsigned int CANVAS_WIDTH = 2;
    const unsigned int CANVAS_HEIGHT = 2;

    gStyle->SetOptStat(0);

    for(auto& angle : config.angles)
    {

        TCanvas* c = new TCanvas(to_string(angle.value).c_str(),"",CANVAS_WIDTH*480,CANVAS_HEIGHT*300);

        c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);

        for(int target=0; target<CANVAS_WIDTH; target++)
        {
            string targetDirName = "../configuration/" + config.experiment
                + "/targets/" + TARGET_NAMES[target] + "/";
            angle.targets.push_back(Target(targetDirName));
        }

        stringstream ss;
        ss << setprecision(5) << angle.value;

        for(int target=0; target<CANVAS_WIDTH; target++)
        {
            auto& t = angle.targets[target];

            string fileName = "../processedData/" + config.experiment + "/angles/" + ss.str() + "/" + t.name + ".root";
            TFile* file = new TFile(fileName.c_str(),"READ");

            if(!file->IsOpen())
            {
                cerr << "Error: couldn't open " << fileName << endl;
                return 1;
            }

            for(int detector=0; detector<CANVAS_HEIGHT; detector++)
            {
                auto& d = config.detectors[detector];

                c->cd(2*detector+target+1);

                string targetHistoName = "histo" + d.name + "Total";
                string blankHistoName = "blank" + d.name + "Total";
                string differenceHistoName = "diff" + d.name;

                TH1I* targetHisto = (TH1I*)file->Get(targetHistoName.c_str());
                TH1I* blankHisto = (TH1I*)file->Get(blankHistoName.c_str());
                TH1I* differenceHisto = (TH1I*)file->Get(differenceHistoName.c_str());

                if(targetHisto && blankHisto && differenceHisto)
                {
                    // Pad dimensions and margins
                    //gPad->SetPad(0, 0.8, 1, 0);
                    gPad->SetLeftMargin(0.12);
                    gPad->SetRightMargin(0);
                    gPad->SetTopMargin(0.12);
                    gPad->SetBottomMargin(0.12);
                    //gPad->SetTicky(2);

                    // Set histo point and line characteristics
                    targetHisto->SetLineColor(kGray+2);
                    targetHisto->SetLineWidth(2);
                    targetHisto->SetLineStyle(0);
                    targetHisto->SetFillColor(17);

                    blankHisto->SetLineColor(kGray+1);
                    blankHisto->SetLineWidth(2);
                    blankHisto->SetLineStyle(0);
                    blankHisto->SetFillColor(18);

                    differenceHisto->SetLineColor(kRed);
                    differenceHisto->SetLineWidth(2);
                    differenceHisto->SetLineStyle(0);
                    differenceHisto->SetFillColor(kRed-10);

                    // X-axis parameters
                    targetHisto->GetXaxis()->SetTitle("TOF (ns)");
                    targetHisto->GetXaxis()->SetTitleSize(0.04);
                    targetHisto->GetXaxis()->SetTitleFont(2);
                    targetHisto->GetXaxis()->SetTitleOffset(1.4);
                    targetHisto->GetXaxis()->CenterTitle();

                    targetHisto->GetXaxis()->SetLabelOffset(0.01);
                    targetHisto->GetXaxis()->SetLabelSize(0.03);
                    targetHisto->GetXaxis()->SetLabelFont(2);

                    targetHisto->GetXaxis()->SetNdivisions(10);
                    targetHisto->GetXaxis()->SetTickLength(0.03);

                    // Y-axis parameters
                    targetHisto->GetYaxis()->SetTitle("Counts");
                    targetHisto->GetYaxis()->SetTitleSize(0.04);
                    targetHisto->GetYaxis()->SetTitleFont(2);
                    targetHisto->GetYaxis()->SetTitleOffset(1.3);
                    targetHisto->GetYaxis()->CenterTitle();

                    targetHisto->GetYaxis()->SetLabelOffset(0.01);
                    targetHisto->GetYaxis()->SetLabelSize(0.03);

                    targetHisto->GetYaxis()->SetLabelFont(2);
                    targetHisto->GetYaxis()->SetNdivisions(10);
                    targetHisto->GetYaxis()->SetTickLength(0);

                    string title = ss.str() + " deg, " + t.name + ", " + d.name;
                    targetHisto->SetTitle(title.c_str());

                    targetHisto->Draw("hist");
                    blankHisto->Draw("hist same");
                    differenceHisto->Draw("hist same");

                    double TOF = calculateTOF(d.distance, 0, t.getMolarMass(), angle.value, config.neutronEnergy);
                    double TOFResolution = d.resolution*d.linearCalibration; // FWHM, in ns

                    double yAxisLowEdge = targetHisto->GetMinimum();
                    double yAxisHighEdge = targetHisto->GetMaximum();

                    double tenthOfPlot = 0.1*(targetHisto->GetMaximum()-targetHisto->GetMinimum());

                    double maxExcitedTOF = 0;
                    for(int state=0; state<t.excitedStates.size(); state++)
                    {
                        double excitedTOF = calculateTOF(d.distance, t.excitedStates[state], t.getMolarMass(), angle.value, config.neutronEnergy);

                        if(maxExcitedTOF<excitedTOF)
                        {
                            maxExcitedTOF = excitedTOF;
                        }

                        double numberOfCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(excitedTOF));
                        TArrow *arrow = new TArrow(excitedTOF, tenthOfPlot+numberOfCounts, excitedTOF, numberOfCounts, 0.015, "|>");
                        arrow->SetAngle(30);
                        arrow->SetLineWidth(3);
                        arrow->SetLineColor(kBlue-9);
                        arrow->SetFillColor(kBlue-9);
                        arrow->Draw();
                    }

                    double nitrogenTOF = calculateTOF(d.distance, 0, 14.001, angle.value, config.neutronEnergy);
                    double nitrogenCounts = blankHisto->GetBinContent(blankHisto->FindBin(nitrogenTOF));
                    TArrow *arrow = new TArrow(nitrogenTOF, tenthOfPlot+nitrogenCounts, nitrogenTOF, nitrogenCounts, 0.015, "|>");
                    arrow->SetAngle(30);
                    arrow->SetLineWidth(3);
                    arrow->SetLineColor(kGreen+2);
                    arrow->SetFillColor(kGreen+2);
                    arrow->Draw();

                    double counts = differenceHisto->GetBinContent(differenceHisto->FindBin(TOF));
                    TArrow *elasticArrow = new TArrow(TOF, tenthOfPlot+counts, TOF, counts, 0.015, "|>");
                    elasticArrow->SetAngle(30);
                    elasticArrow->SetLineWidth(3);
                    elasticArrow->SetLineColor(kBlue+2);
                    elasticArrow->SetFillColor(kBlue+2);
                    elasticArrow->Draw();

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

                    double maxTOFRange = max(nitrogenTOF, maxExcitedTOF);
                    targetHisto->GetXaxis()->SetRangeUser(TOF-10, maxTOFRange+10);
                    targetHisto->GetYaxis()->SetRangeUser(differenceHisto->GetMinimum(), 1.1*targetHisto->GetMaximum());
                }

                else
                {
                    TLatex latex;
                    latex.SetNDC();
                    latex.SetTextSize(0.06);
                    latex.SetTextAlign(13); // align at top
                    latex.SetTextColor(kGray+2);

                    string title = "No data for " + ss.str() + " deg, " + t.name + " " + d.name;
                    latex.DrawLatex(0.3,0.50,title.c_str());
                }
            }
        }

        c->Update();
        TImage *img = TImage::Create();

        img->FromPad(c);

        string name = "../processedData/" + config.experiment + "/plots/" + ss.str() + "Deg.png";
        img->WriteImage(name.c_str());

        delete c;
    }

    return 0;
}

int plotReference(const ReferenceCS& reference)
{
    const unsigned int CANVAS_WIDTH = 1;
    const unsigned int CANVAS_HEIGHT = 2;

    gStyle->SetOptStat(0);

    TCanvas* c = new TCanvas(to_string(reference.angle).c_str(),"",CANVAS_WIDTH*480,CANVAS_HEIGHT*300);

    c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);

    stringstream ss;
    ss << setprecision(5) << reference.angle;

    string fileName = "../configuration/" + config.experiment + "/normalization/" + reference.name + ".root";
    TFile* file = new TFile(fileName.c_str(),"READ");

    if(!file->IsOpen())
    {
        cerr << "Error: couldn't open " << fileName << endl;
        return 1;
    }

    for(int detector=0; detector<CANVAS_HEIGHT; detector++)
    {
        auto& d = config.detectors[detector];

        c->cd(detector+1);

        string polyHistoName = "polyMinusBlank" + d.name;
        string graphiteHistoName = "graphiteMinusBlank" + d.name;
        string differenceHistoName = "polyMinusGraphite" + d.name;

        TH1I* polyHisto = (TH1I*)file->Get(polyHistoName.c_str());
        TH1I* graphiteHisto = (TH1I*)file->Get(graphiteHistoName.c_str());
        TH1I* differenceHisto = (TH1I*)file->Get(differenceHistoName.c_str());

        if(polyHisto && graphiteHisto && differenceHisto)
        {
            // Pad dimensions and margins
            //gPad->SetPad(0, 0.8, 1, 0);
            gPad->SetLeftMargin(0.12);
            gPad->SetRightMargin(0);
            gPad->SetTopMargin(0.12);
            gPad->SetBottomMargin(0.12);
            //gPad->SetTicky(2);

            // Set histo point and line characteristics
            polyHisto->SetLineColor(kGray+2);
            polyHisto->SetLineWidth(2);
            polyHisto->SetLineStyle(0);
            polyHisto->SetFillColor(17);

            graphiteHisto->SetLineColor(kGray+1);
            graphiteHisto->SetLineWidth(2);
            graphiteHisto->SetLineStyle(0);
            graphiteHisto->SetFillColor(18);

            differenceHisto->SetLineColor(kRed);
            differenceHisto->SetLineWidth(2);
            differenceHisto->SetLineStyle(0);
            differenceHisto->SetFillColor(kRed-10);

            // X-axis parameters
            polyHisto->GetXaxis()->SetTitle("TOF (ns)");
            polyHisto->GetXaxis()->SetTitleSize(0.04);
            polyHisto->GetXaxis()->SetTitleFont(2);
            polyHisto->GetXaxis()->SetTitleOffset(1.4);
            polyHisto->GetXaxis()->CenterTitle();

            polyHisto->GetXaxis()->SetLabelOffset(0.01);
            polyHisto->GetXaxis()->SetLabelSize(0.03);
            polyHisto->GetXaxis()->SetLabelFont(2);

            polyHisto->GetXaxis()->SetNdivisions(10);
            polyHisto->GetXaxis()->SetTickLength(0.03);

            // Y-axis parameters
            polyHisto->GetYaxis()->SetTitle("Counts");
            polyHisto->GetYaxis()->SetTitleSize(0.04);
            polyHisto->GetYaxis()->SetTitleFont(2);
            polyHisto->GetYaxis()->SetTitleOffset(1.3);
            polyHisto->GetYaxis()->CenterTitle();

            polyHisto->GetYaxis()->SetLabelOffset(0.01);
            polyHisto->GetYaxis()->SetLabelSize(0.03);

            polyHisto->GetYaxis()->SetLabelFont(2);
            polyHisto->GetYaxis()->SetNdivisions(10);
            polyHisto->GetYaxis()->SetTickLength(0);

            string title = "Reference CS for " + d.name;
            polyHisto->SetTitle(title.c_str());

            polyHisto->Draw("hist");
            graphiteHisto->Draw("hist same");
            differenceHisto->Draw("hist same");

            double tenthOfPlot = 0.1*(polyHisto->GetMaximum()-polyHisto->GetMinimum());

            double carbonTOF = calculateTOF(d.distance, 0, 12.011, reference.angle, config.neutronEnergy);
            double carbonCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(carbonTOF));
            TArrow *carbonArrow = new TArrow(carbonTOF, tenthOfPlot+carbonCounts, carbonTOF, carbonCounts, 0.015, "|>");
            carbonArrow->SetAngle(30);
            carbonArrow->SetLineWidth(3);
            carbonArrow->SetLineColor(kBlue-9);
            carbonArrow->SetFillColor(kBlue-9);
            carbonArrow->Draw();

            double carbonExcitedTOF = calculateTOF(d.distance, 4.4389, 12.011, reference.angle, config.neutronEnergy);
            double carbonExcitedCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(carbonExcitedTOF));
            TArrow *carbonExcitedArrow = new TArrow(carbonExcitedTOF, tenthOfPlot+carbonExcitedCounts, carbonExcitedTOF, carbonExcitedCounts, 0.015, "|>");
            carbonExcitedArrow->SetAngle(30);
            carbonExcitedArrow->SetLineWidth(3);
            carbonExcitedArrow->SetLineColor(kBlue-9);
            carbonExcitedArrow->SetFillColor(kBlue-9);
            carbonExcitedArrow->Draw();

            double nitrogenTOF = calculateTOF(d.distance, 0, 14.001, reference.angle, config.neutronEnergy);
            double nitrogenCounts = graphiteHisto->GetBinContent(graphiteHisto->FindBin(nitrogenTOF));
            TArrow *nitrogenArrow = new TArrow(nitrogenTOF, tenthOfPlot+nitrogenCounts, nitrogenTOF, nitrogenCounts, 0.015, "|>");
            nitrogenArrow->SetAngle(30);
            nitrogenArrow->SetLineWidth(3);
            nitrogenArrow->SetLineColor(kGreen+2);
            nitrogenArrow->SetFillColor(kGreen+2);
            nitrogenArrow->Draw();

            double hydrogenTOF = calculateTOF(d.distance, 0, 1.007279, reference.angle, config.neutronEnergy);
            double TOFResolution = d.resolution*d.linearCalibration; // FWHM, in ns

            double hydrogenCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(hydrogenTOF));
            TArrow *hydrogenArrow = new TArrow(hydrogenTOF, tenthOfPlot+hydrogenCounts, hydrogenTOF, hydrogenCounts, 0.015, "|>");
            hydrogenArrow->SetAngle(30);
            hydrogenArrow->SetLineWidth(3);
            hydrogenArrow->SetLineColor(kBlue+2);
            hydrogenArrow->SetFillColor(kBlue+2);
            hydrogenArrow->Draw();

            double yAxisLowEdge = differenceHisto->GetMinimum();
            double yAxisHighEdge = differenceHisto->GetMaximum();

            TLine *gateLowLine = new TLine(hydrogenTOF-TOFResolution,yAxisLowEdge,hydrogenTOF-TOFResolution,yAxisHighEdge);
            TLine *gateHighLine = new TLine(hydrogenTOF+TOFResolution,yAxisLowEdge,hydrogenTOF+TOFResolution,yAxisHighEdge);
            gateLowLine->SetLineStyle(7);
            gateLowLine->SetLineWidth(3);
            gateLowLine->SetLineColor(kBlue);
            gateLowLine->Draw();

            gateHighLine->SetLineStyle(7);
            gateHighLine->SetLineWidth(3);
            gateHighLine->SetLineColor(kBlue);
            gateHighLine->Draw();

            polyHisto->GetXaxis()->SetRangeUser(carbonTOF-10, carbonExcitedTOF+10);
            polyHisto->GetYaxis()->SetRangeUser(differenceHisto->GetMinimum(), 1.1*polyHisto->GetMaximum());
        }

        else
        {
            cerr << "Error: couldn't find histograms for producing reference histo plots." << endl;
            return 1;
        }
    }

    c->Update();
    TImage *img = TImage::Create();

    img->FromPad(c);

    string name = "../processedData/" + config.experiment + "/plots/reference" + reference.name + ".png";
    img->WriteImage(name.c_str());

    return 0;
}
