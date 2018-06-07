#include "../include/Config.h"
#include "../include/utilities.h"
#include "../include/experimentalConstants.h"
#include "../include/ReferenceCS.h"
#include "../include/physicalConstants.h"

#include "TCanvas.h"
#include "TH1.h"
#include "TLine.h"
#include "TImage.h"
#include "TFile.h"
#include "TArrow.h"
#include "TLatex.h"
#include "TStyle.h"
#include "TF1.h"
#include "TGaxis.h"
#include "TGraph.h"

#include <sstream>

using namespace std;

extern Config config;

const double EFFICIENCY_PAD_FRACTION = 0.3;
const double PAD_MARGIN = 0.015;

int plotHistos()
{
    const unsigned int DETECTORS_PER_RUN = 2;
    const unsigned int RUNS_PER_CANVAS = 3;

    TCanvas* c;

    for(int i=0; i<config.runs.size(); i++)
    {
        auto& t = config.runs[i].target;

        if(i%RUNS_PER_CANVAS==0)
        {
            c = new TCanvas(to_string(config.runs[i].number).c_str(),"",DETECTORS_PER_RUN*700,RUNS_PER_CANVAS*600);
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

            c->cd();
            TPad *histoPad = new TPad(
                    "histoPad",
                    "",
                    detector/(double)DETECTORS_PER_RUN+PAD_MARGIN,
                    1-(i%RUNS_PER_CANVAS+1)/(double)RUNS_PER_CANVAS+PAD_MARGIN,
                    (detector+1)/(double)DETECTORS_PER_RUN-PAD_MARGIN,
                    1+(1-EFFICIENCY_PAD_FRACTION)/RUNS_PER_CANVAS-(i%RUNS_PER_CANVAS+1)/(double)RUNS_PER_CANVAS,
                    0);
            histoPad->Draw();

            TPad *efficiencyPad = new TPad(
                    "efficiencyPad",
                    "",
                    detector/(double)DETECTORS_PER_RUN+PAD_MARGIN,
                    1+(1-EFFICIENCY_PAD_FRACTION)/RUNS_PER_CANVAS-(i%RUNS_PER_CANVAS+1)/(double)RUNS_PER_CANVAS,
                    (detector+1)/(double)DETECTORS_PER_RUN-PAD_MARGIN,
                    1-(i%RUNS_PER_CANVAS)/(double)RUNS_PER_CANVAS-PAD_MARGIN,
                    0);
            efficiencyPad->Draw();

            histoPad->cd();
            gPad->SetLeftMargin(0.15);
            gPad->SetRightMargin(0.1);
            gPad->SetTopMargin(0.0);
            gPad->SetBottomMargin(0.18);
            gPad->SetTickx(2);

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

            string TOFName = d.name + "TOF_EC";
            TH1D* TOFHisto = (TH1D*)file->Get(TOFName.c_str());

            if(TOFHisto)
            {
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

                TOFHisto->GetYaxis()->SetLabelOffset(0.02);
                TOFHisto->GetYaxis()->SetLabelSize(0.06);

                //TOFHisto->GetYaxis()->SetLabelFont(2);
                TOFHisto->GetYaxis()->SetNdivisions(5);
                TOFHisto->GetYaxis()->SetTickLength(0.02);

                gStyle->SetOptStat(0);

                double nitrogenTOF = calculateTOF(d.distance, 0, 14.001, angle, config.neutronEnergy);

                double TOF = calculateTOF(d.distance, 0, t.getMolarMass(), angle, config.neutronEnergy);
                double TOFResolution = d.resolution*d.linearCalibration; // FWHM, in ns

                double maxExcitedTOF = 0;
                for(int state=0; state<t.excitedStates.size(); state++)
                {
                    double excitedTOF = calculateTOF(d.distance, t.excitedStates[state], t.getMolarMass(), angle, config.neutronEnergy);

                    if(maxExcitedTOF<excitedTOF)
                    {
                        maxExcitedTOF = excitedTOF;
                    }
                }

                double maxTOFRange = max(nitrogenTOF, maxExcitedTOF);
                TOFHisto->SetTitle("");
                TOFHisto->Draw("hist");

                int xLowBin = TOFHisto->GetXaxis()->FindBin(TOF-5);
                int xHighBin = TOFHisto->GetXaxis()->FindBin(maxTOFRange+10);

                double xLow = TOFHisto->GetXaxis()->GetBinLowEdge(xLowBin);
                double xHigh = TOFHisto->GetXaxis()->GetBinLowEdge(xHighBin)
                    +TOFHisto->GetXaxis()->GetBinWidth(xHighBin);

                /*cout << "original low limit = " << TOF-5 << ", calculated limit = " << xLow
                    << ", original high limit = " << maxTOFRange+10 << ", calculated limit = " << xHigh << endl;
                    */

                TOFHisto->GetXaxis()->SetRange(xLowBin, xHighBin);

                double histoMinValue = TOFHisto->GetMinimum();
                double histoMaxValue = 1.5*TOFHisto->GetMaximum();

                for(int state=0; state<t.excitedStates.size(); state++)
                {
                    double excitedTOF = calculateTOF(d.distance, t.excitedStates[state], t.getMolarMass(), angle, config.neutronEnergy);

                    double numberOfCounts = TOFHisto->GetBinContent(TOFHisto->GetBin(excitedTOF));
                    TArrow *arrow = new TArrow(excitedTOF, histoMaxValue, excitedTOF, numberOfCounts, 0.015, "|>");
                    arrow->SetAngle(30);
                    arrow->SetLineWidth(3);
                    arrow->SetLineColor(kRed+2);
                    arrow->SetFillColor(kRed+2);
                    arrow->Draw();
                }

                double nitrogenCounts = TOFHisto->GetBinContent(TOFHisto->GetBin(nitrogenTOF));
                TArrow *arrow = new TArrow(nitrogenTOF, histoMaxValue, nitrogenTOF, nitrogenCounts, 0.015, "|>");
                arrow->SetAngle(30);
                arrow->SetLineWidth(3);
                arrow->SetLineColor(kGreen+2);
                arrow->SetFillColor(kGreen+2);
                arrow->Draw();

                double counts = TOFHisto->GetBinContent(TOFHisto->GetBin(TOF));
                TArrow *elasticArrow = new TArrow(TOF, histoMaxValue, TOF, counts, 0.015, "|>");
                elasticArrow->SetAngle(30);
                elasticArrow->SetLineWidth(3);
                elasticArrow->SetLineColor(kBlue+2);
                elasticArrow->SetFillColor(kBlue+2);
                elasticArrow->Draw();

                TLine *gateLowLine = new TLine(TOF-TOFResolution,histoMinValue,TOF-TOFResolution,histoMaxValue);
                TLine *gateHighLine = new TLine(TOF+TOFResolution,histoMinValue,TOF+TOFResolution,histoMaxValue);
                gateLowLine->SetLineStyle(7);
                gateLowLine->SetLineWidth(3);
                gateLowLine->SetLineColor(kBlue);
                gateLowLine->Draw();

                gateHighLine->SetLineStyle(7);
                gateHighLine->SetLineWidth(3);
                gateHighLine->SetLineColor(kBlue);
                gateHighLine->Draw();

                // add efficiency to plot
                vector<double> TOFValues;
                vector<double> efficiencyValues;

                //double referenceTOF = calculateTOF(d.distance, 0, 100000, angle, config.neutronEnergy);
                //double refVelocity = ((d.distance/referenceTOF)*pow(10,7)/C);
                //double refEnergy = 0.5*NEUTRON_MASS*AMU_TO_MEVC2*pow(refVelocity,2);
                double refEfficiency = d.efficiency.getEfficiency(17);

                for(int tof=TOF-5; tof<maxTOFRange+10; tof++)
                {
                    double velocity = ((d.distance/tof)*pow(10,7)/C);
                    if(velocity < 0 || velocity > 1)
                    {
                        continue;
                    }

                    double energy = 0.5*NEUTRON_MASS*AMU_TO_MEVC2*pow(velocity,2);
                    if(energy<1 || energy>19)
                    {
                        continue;
                    }

                    double efficiency = d.efficiency.getEfficiency(energy);

                    if(efficiency <= 0 || efficiency >1)
                    {
                        continue;
                    }

                    else
                    {
                        TOFValues.push_back(tof);
                        efficiencyValues.push_back(efficiency/refEfficiency);
                    }
                }

                double minimumEfficiency = 1;
                double maximumEfficiency = 1;

                for(auto& eff : efficiencyValues)
                {
                    if(eff > maximumEfficiency)
                    {
                        maximumEfficiency = eff;
                    }

                    if(eff < minimumEfficiency)
                    {
                        minimumEfficiency = eff;
                    }
                }

                efficiencyPad->cd();
                gPad->SetLeftMargin(0.15);
                gPad->SetRightMargin(0.1);
                gPad->SetTopMargin(0.25);
                gPad->SetBottomMargin(0.0);
                //gPad->SetTicky(2);

                TGraph* efficiencyGraph = new TGraph(TOFValues.size(), &TOFValues[0], &efficiencyValues[0]);
                string title = "Run " + to_string(config.runs[i].number) + " (" + t.name + "), " + d.name + " TOF";
                efficiencyGraph->SetTitle(title.c_str());

                gStyle->SetTitleH(0.2);
                gStyle->SetTitleAlign(23);
                efficiencyGraph->SetLineWidth(3);
                efficiencyGraph->SetLineColor(kRed);

                efficiencyGraph->GetXaxis()->SetNdivisions(10);
                efficiencyGraph->GetXaxis()->SetLabelSize(0);
                efficiencyGraph->GetXaxis()->SetTickLength(0.03);
                efficiencyGraph->GetXaxis()->SetTickLength(0.03);
                
                efficiencyGraph->GetYaxis()->SetNdivisions(3);
                efficiencyGraph->GetYaxis()->SetLabelSize(0.15);
                efficiencyGraph->GetYaxis()->SetTitle("Efficiency");
                efficiencyGraph->GetYaxis()->SetTitleSize(0.15);
                efficiencyGraph->GetYaxis()->SetTitleOffset(0.5);
                efficiencyGraph->GetYaxis()->SetTitleFont(2);
                efficiencyGraph->GetYaxis()->CenterTitle();

                efficiencyGraph->Draw();
                efficiencyGraph->GetXaxis()->SetLimits(xLow, xHigh);

                TLine *zeroEffLine = new TLine(TOF-5,1,maxTOFRange+10,1);
                zeroEffLine->SetLineStyle(2);
                zeroEffLine->SetLineColor(kGray+2);

                zeroEffLine->Draw();
            }
        }

        if(i%RUNS_PER_CANVAS==(RUNS_PER_CANVAS-1))
        {
            c->Update();
            TImage *img = TImage::Create();

            img->FromPad(c);

            string name = "../processedData/" + config.experiment + "/plots/set" + to_string(i/RUNS_PER_CANVAS) + ".png";
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

        stringstream ss;
        ss << setprecision(5) << angle.value;

        for(auto& t : angle.targets)
        {
            int target;

            if(t.name==TARGET_NAMES[0])
            {
                target = 0;
            }

            else if(t.name==TARGET_NAMES[1])
            {
                target = 1;
            }

            else
            {
                continue;
            }

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

                TH1D* targetHisto = (TH1D*)file->Get(targetHistoName.c_str());
                TH1D* blankHisto = (TH1D*)file->Get(blankHistoName.c_str());

                string monitorName = "monitor" + d.name + "Total";
                TH1D* monitorHisto = (TH1D*)file->Get(monitorName.c_str());

                string blankMonitorName = "blankMonitor" + d.name + "Total";
                TH1D* blankMonitorHisto = (TH1D*)file->Get(blankMonitorName.c_str());

                if(targetHisto && blankHisto && monitorHisto && blankMonitorHisto)
                {
                    // calculate and plot difference histogram
                    targetHisto->Scale(
                            NORMALIZATION_FACTOR/monitorHisto->GetEntries());
                    blankHisto->Scale(
                            NORMALIZATION_FACTOR/blankMonitorHisto->GetEntries());

                    string diffName = "diff" + d.name;
                    TH1D* differenceHisto = (TH1D*)targetHisto->Clone(diffName.c_str());

                    differenceHisto->Add(blankHisto, -1);

                    // Pad dimensions and margins
                    //gPad->SetPad(0, 0.8, 1, 0);
                    gPad->SetLeftMargin(0.12);
                    gPad->SetRightMargin(0);
                    gPad->SetTopMargin(0.15);
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
                    targetHisto->SetTitleOffset(1.2);
                    gStyle->SetTitleH(0.1);
                    gStyle->SetTitleAlign(23);

                    targetHisto->Draw("hist");
                    blankHisto->Draw("hist same");
                    differenceHisto->Draw("hist same");

                    double TOF = calculateTOF(d.distance, 0, t.getMolarMass(), angle.value, config.neutronEnergy);
                    double TOFResolution = d.resolution*d.linearCalibration; // FWHM, in ns

                    double yAxisLowEdge = targetHisto->GetMinimum();
                    double yAxisHighEdge = targetHisto->GetMaximum();

                    double maxExcitedTOF = 0;
                    for(int state=0; state<t.excitedStates.size(); state++)
                    {
                        double excitedTOF = calculateTOF(d.distance, t.excitedStates[state], t.getMolarMass(), angle.value, config.neutronEnergy);

                        if(maxExcitedTOF<excitedTOF)
                        {
                            maxExcitedTOF = excitedTOF;
                        }

                        double numberOfCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(excitedTOF));
                        TArrow *arrow = new TArrow(excitedTOF, 1+numberOfCounts, excitedTOF, numberOfCounts, 0.015, "|>");
                        arrow->SetAngle(30);
                        arrow->SetLineWidth(3);
                        arrow->SetLineColor(kBlue-9);
                        arrow->SetFillColor(kBlue-9);
                        arrow->Draw();
                    }

                    double nitrogenTOF = calculateTOF(d.distance, 0, 14.001, angle.value, config.neutronEnergy);
                    double nitrogenCounts = blankHisto->GetBinContent(blankHisto->FindBin(nitrogenTOF));
                    TArrow *arrow = new TArrow(nitrogenTOF, 1+nitrogenCounts, nitrogenTOF, nitrogenCounts, 0.015, "|>");
                    arrow->SetAngle(30);
                    arrow->SetLineWidth(3);
                    arrow->SetLineColor(kGreen+2);
                    arrow->SetFillColor(kGreen+2);
                    arrow->Draw();

                    double counts = differenceHisto->GetBinContent(differenceHisto->FindBin(TOF));
                    TArrow *elasticArrow = new TArrow(TOF, 1+counts, TOF, counts, 0.015, "|>");
                    elasticArrow->SetAngle(30);
                    elasticArrow->SetLineWidth(3);
                    elasticArrow->SetLineColor(kBlue+2);
                    elasticArrow->SetFillColor(kBlue+2);
                    elasticArrow->Draw();

                    TF1* fit = new TF1("gaussian","gaus",TOF-3, TOF+3);
                    fit->SetParLimits(1, TOF-1, TOF+1);
                    fit->SetParameter(1, TOF);
                    fit->SetParameter(0, counts);
                    fit->SetParameter(2, TOFResolution/2.355);

                    differenceHisto->Fit("gaussian","BQ", "",TOF-3, TOF+3);
                    fit->Draw("same");

                    TLine *gateLowLine = new TLine(TOF-TOFResolution,yAxisLowEdge,TOF-TOFResolution, counts);
                    TLine *gateHighLine = new TLine(TOF+TOFResolution,yAxisLowEdge,TOF+TOFResolution, counts);
                    gateLowLine->SetLineStyle(7);
                    gateLowLine->SetLineWidth(3);
                    gateLowLine->SetLineColor(kBlue);
                    gateLowLine->Draw();

                    gateHighLine->SetLineStyle(7);
                    gateHighLine->SetLineWidth(3);
                    gateHighLine->SetLineColor(kBlue);
                    gateHighLine->Draw();

                    double maxTOFRange = max(nitrogenTOF, maxExcitedTOF);
                    targetHisto->GetXaxis()->SetRangeUser(TOF-5, maxTOFRange+10);
                    targetHisto->GetYaxis()->SetRangeUser(-0.1*targetHisto->GetMaximum(), 1.1*targetHisto->GetMaximum());
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

int plotReference(const vector<vector<ReferenceCS>>& references)
{
    const unsigned int CANVAS_WIDTH = 1;
    const unsigned int CANVAS_HEIGHT = 2;

    gStyle->SetOptStat(0);
    
    for(int i=0; i<references.back().size(); i++)
    {
        TCanvas* c = new TCanvas(to_string(references.back()[i].angle).c_str(),"",CANVAS_WIDTH*480,CANVAS_HEIGHT*300);

        c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);

        stringstream ss;
        ss << setprecision(5) << references.back()[i].angle;

        for(int detector=0; detector<CANVAS_HEIGHT; detector++)
        {
            auto& d = config.detectors[detector];

            c->cd(detector+1);

            string fileName = "../configuration/" + config.experiment
                + "/normalization/" + references.back()[i].name
                + "_" + d.name + ".root";

            TFile* file = new TFile(fileName.c_str(),"READ");

            if(!file->IsOpen())
            {
                cerr << "Error: couldn't open " << fileName << " during plotReferences" << endl;
                return 1;
            }

            string polyHistoName = "polyMinusBlank" + d.name;
            string graphiteHistoName = "graphiteMinusBlank" + d.name;
            string differenceHistoName = "polyMinusGraphite" + d.name;

            TH1D* polyHisto = (TH1D*)file->Get(polyHistoName.c_str());
            TH1D* graphiteHisto = (TH1D*)file->Get(graphiteHistoName.c_str());
            TH1D* differenceHisto = (TH1D*)file->Get(differenceHistoName.c_str());

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

                double carbonTOF = calculateTOF(d.distance, 0, 12.011, references[detector][i].angle, config.neutronEnergy);
                double carbonCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(carbonTOF));
                TArrow *carbonArrow = new TArrow(carbonTOF, tenthOfPlot+carbonCounts, carbonTOF, carbonCounts, 0.015, "|>");
                carbonArrow->SetAngle(30);
                carbonArrow->SetLineWidth(3);
                carbonArrow->SetLineColor(kBlue-9);
                carbonArrow->SetFillColor(kBlue-9);
                carbonArrow->Draw();

                double carbonExcitedTOF = calculateTOF(d.distance, 4.4389, 12.011, references[detector][i].angle, config.neutronEnergy);
                double carbonExcitedCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(carbonExcitedTOF));
                TArrow *carbonExcitedArrow = new TArrow(carbonExcitedTOF, tenthOfPlot+carbonExcitedCounts, carbonExcitedTOF, carbonExcitedCounts, 0.015, "|>");
                carbonExcitedArrow->SetAngle(30);
                carbonExcitedArrow->SetLineWidth(3);
                carbonExcitedArrow->SetLineColor(kBlue-9);
                carbonExcitedArrow->SetFillColor(kBlue-9);
                carbonExcitedArrow->Draw();

                double nitrogenTOF = calculateTOF(d.distance, 0, 14.001, references[detector][i].angle, config.neutronEnergy);
                double nitrogenCounts = graphiteHisto->GetBinContent(graphiteHisto->FindBin(nitrogenTOF));
                TArrow *nitrogenArrow = new TArrow(nitrogenTOF, tenthOfPlot+nitrogenCounts, nitrogenTOF, nitrogenCounts, 0.015, "|>");
                nitrogenArrow->SetAngle(30);
                nitrogenArrow->SetLineWidth(3);
                nitrogenArrow->SetLineColor(kGreen+2);
                nitrogenArrow->SetFillColor(kGreen+2);
                nitrogenArrow->Draw();

                double hydrogenTOF = calculateTOF(d.distance, 0, 1.007279, references[detector][i].angle, config.neutronEnergy);

                double hydrogenCounts = differenceHisto->GetBinContent(differenceHisto->FindBin(hydrogenTOF));
                TArrow *hydrogenArrow = new TArrow(hydrogenTOF, tenthOfPlot+hydrogenCounts, hydrogenTOF, hydrogenCounts, 0.015, "|>");
                hydrogenArrow->SetAngle(30);
                hydrogenArrow->SetLineWidth(3);
                hydrogenArrow->SetLineColor(kBlue+2);
                hydrogenArrow->SetFillColor(kBlue+2);
                hydrogenArrow->Draw();

                double yAxisLowEdge = differenceHisto->GetMinimum();
                double yAxisHighEdge = differenceHisto->GetMaximum();

                double lowIntLimit = references[detector][i].intLimits[0];
                double highIntLimit = references[detector][i].intLimits[1];

                TLine *gateLowLine = new TLine(lowIntLimit,yAxisLowEdge,lowIntLimit,yAxisHighEdge);
                TLine *gateHighLine = new TLine(highIntLimit,yAxisLowEdge,highIntLimit,yAxisHighEdge);
                gateLowLine->SetLineStyle(7);
                gateLowLine->SetLineWidth(3);
                gateLowLine->SetLineColor(kBlue);
                gateLowLine->Draw();

                gateHighLine->SetLineStyle(7);
                gateHighLine->SetLineWidth(3);
                gateHighLine->SetLineColor(kBlue);
                gateHighLine->Draw();

                polyHisto->GetXaxis()->SetRangeUser(carbonTOF-5, carbonExcitedTOF+10);
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

        string name = "../processedData/" + config.experiment + "/plots/reference" + references.back()[i].name + ".png";
        img->WriteImage(name.c_str());
    }

    return 0;
}
