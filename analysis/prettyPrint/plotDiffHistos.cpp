const double FOUR_M_INT_LIMIT_MIN = 1285;
const double FOUR_M_INT_LIMIT_MAX = 1315;

const double SIX_M_INT_LIMIT_MIN = 940;
const double SIX_M_INT_LIMIT_MAX = 985;

void plotDiffHistos(string angle)
{
    const unsigned int CANVAS_WIDTH = 2;
    const unsigned int CANVAS_HEIGHT = 2;

    TStyle * style = (TStyle*)gROOT->FindObject("histoStyle");

    if(!style)      
    {
        style = new TStyle("histoStyle","histoStyle");
    }

    style->SetOptStat(0);
    style->SetOptTitle(1);    
    style->SetPalette(1,0);
    style->SetCanvasColor(10);      
    style->SetCanvasBorderMode(0);    
    style->SetFrameLineWidth(3);
    style->SetFrameFillColor(10);
    style->SetPadColor(10);
    style->SetHistLineWidth(3);
    style->SetHistLineColor(kBlue);
    style->SetFuncWidth(3);
    style->SetFuncColor(kRed);
    style->SetLabelColor(kBlack,"xyz");
    style->SetTitleSize(0.09,"t");
    style->SetTitleBorderSize(0);
    style->SetTitleFillColor(10);
    style->SetTitleTextColor(kBlack);
    style->SetTitleX(0.5);
    style->SetTitleAlign(23);
    style->SetEndErrorSize(0);

    gROOT->SetStyle("histoStyle");
    gROOT->ForceStyle();

    TCanvas* c = new TCanvas("c1","",CANVAS_WIDTH*500,CANVAS_HEIGHT*300);

    c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);

    string Sn112FileName = "../../processedData/angles/" + angle + "/Sn112.root";
    TFile* Sn112File = new TFile(Sn112FileName.c_str(),"READ");

    string Sn124FileName = "../../processedData/angles/" + angle + "/Sn124.root";
    TFile* Sn124File = new TFile(Sn124FileName.c_str(),"READ");

    if(!Sn112File->IsOpen() || !Sn124File->IsOpen())
    {
        cerr << "Error: couldn't open either " << Sn112FileName << " or "
            << Sn124FileName << endl;
        exit(1);
    }

    {
        // First quadrant: 4M det, Sn112
        c->cd(1);

        string targetHistoName = "histo4MTotal";
        string blankHistoName = "blank4MTotal";
        string differenceHistoName = "diff4M";

        TH1* targetHisto = (TH1*)Sn112File->Get(targetHistoName.c_str());
        TH1* blankHisto = (TH1*)Sn112File->Get(blankHistoName.c_str());
        TH1* differenceHisto = (TH1*)Sn112File->Get(differenceHistoName.c_str());

        if(targetHisto && blankHisto && differenceHisto)
        {
            // Pad dimensions and margins
            //gPad->SetPad(0, 0.8, 1, 0);
            gPad->SetLeftMargin(0.12);
            gPad->SetRightMargin(0.10);
            gPad->SetTopMargin(0.12);
            gPad->SetBottomMargin(0.12);
            //gPad->SetTicky(2);

            // Set histo point and line characteristics
            targetHisto->SetLineColor(kBlack);
            targetHisto->SetLineWidth(2);
            targetHisto->SetLineStyle(0);

            blankHisto->SetLineColor(kGray+2);
            blankHisto->SetLineWidth(2);
            blankHisto->SetLineStyle(0);

            differenceHisto->SetLineColor(kRed);
            differenceHisto->SetLineWidth(2);
            differenceHisto->SetLineStyle(0);

            // X-axis parameters
            targetHisto->GetXaxis()->SetTitle("TOF (a.u.)");
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

            string title = angle + " deg, Sn112, 4M";
            targetHisto->SetTitle(title.c_str());

            targetHisto->SetFillColor(19);
            blankHisto->SetFillColor(29);

            targetHisto->Draw("hist");
            blankHisto->Draw("hist same");
            //differenceHisto->Draw("hist same");

            targetHisto->GetXaxis()->SetRangeUser(1220,1350);

            int minIntBin = differenceHisto->FindBin(FOUR_M_INT_LIMIT_MIN);
            int maxIntBin = differenceHisto->FindBin(FOUR_M_INT_LIMIT_MAX);

            ostringstream ss;
            ss << differenceHisto->Integral(minIntBin, maxIntBin);
            string integral = ss.str();

            TLatex latex;
            latex.SetNDC();
            latex.SetTextSize(0.05);
            latex.SetTextAlign(13); // align at top
            latex.SetTextColor(kRed);

            string diffInt = "Integrated diff: ";
            latex.DrawLatex(0.20,0.85,diffInt.c_str());
            latex.DrawLatex(0.20, 0.78, integral.c_str());

            double yAxisLowEdge = targetHisto->GetMinimum();
            double yAxisHighEdge = targetHisto->GetMaximum();

            TLine *gateLowLine = new TLine(FOUR_M_INT_LIMIT_MIN,yAxisLowEdge,FOUR_M_INT_LIMIT_MIN,yAxisHighEdge);
            TLine *gateHighLine = new TLine(FOUR_M_INT_LIMIT_MAX,yAxisLowEdge,FOUR_M_INT_LIMIT_MAX,yAxisHighEdge);
            gateLowLine->SetLineStyle(7);
            gateLowLine->SetLineWidth(3);
            gateLowLine->SetLineColor(kBlue);
            gateLowLine->Draw();

            gateHighLine->SetLineStyle(7);
            gateHighLine->SetLineWidth(3);
            gateHighLine->SetLineColor(kBlue);
            gateHighLine->Draw();
        }

        else
        {
            cout << "For Sn112, no 4M histos found; skipping." << endl;
        }
    }

    {
        // Second quadrant: 6M det, Sn112
        c->cd(2);

        string targetHistoName = "histo6MTotal";
        string blankHistoName = "blank6MTotal";
        string differenceHistoName = "diff6M";

        TH1* targetHisto = (TH1*)Sn112File->Get(targetHistoName.c_str());
        TH1* blankHisto = (TH1*)Sn112File->Get(blankHistoName.c_str());
        TH1* differenceHisto = (TH1*)Sn112File->Get(differenceHistoName.c_str());

        if(targetHisto && blankHisto && differenceHisto)
        {
            // Pad dimensions and margins
            //gPad->SetPad(0, 0.8, 1, 0);
            gPad->SetLeftMargin(0.12);
            gPad->SetRightMargin(0.10);
            gPad->SetTopMargin(0.12);
            gPad->SetBottomMargin(0.12);
            //gPad->SetTicky(2);

            // Set histo point and line characteristics
            targetHisto->SetLineColor(kBlack);
            targetHisto->SetLineWidth(2);
            targetHisto->SetLineStyle(0);

            blankHisto->SetLineColor(kGray+2);
            blankHisto->SetLineWidth(2);
            blankHisto->SetLineStyle(0);

            differenceHisto->SetLineColor(kRed);
            differenceHisto->SetLineWidth(2);
            differenceHisto->SetLineStyle(0);

            // X-axis parameters
            targetHisto->GetXaxis()->SetTitle("TOF (a.u.)");
            targetHisto->GetXaxis()->SetTitleSize(0.04);
            targetHisto->GetXaxis()->SetTitleFont(2);
            targetHisto->GetXaxis()->SetTitleOffset(1.4);
            targetHisto->GetXaxis()->CenterTitle();

            targetHisto->GetXaxis()->SetLabelOffset(0.01);
            targetHisto->GetXaxis()->SetLabelSize(0.03);
            targetHisto->GetXaxis()->SetLabelFont(2);

            targetHisto->GetXaxis()->SetNdivisions(10);
            targetHisto->GetXaxis()->SetTickLength(0.03);

            targetHisto->GetXaxis()->SetRangeUser(900,1020);

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

            string title = angle + " deg, Sn112, 6M";
            targetHisto->SetTitle(title.c_str());

            targetHisto->SetFillColor(19);
            blankHisto->SetFillColor(29);

            targetHisto->Draw("hist");
            blankHisto->Draw("hist same");
            //differenceHisto->Draw("hist same");

            int minIntBin = differenceHisto->FindBin(SIX_M_INT_LIMIT_MIN);
            int maxIntBin = differenceHisto->FindBin(SIX_M_INT_LIMIT_MAX);

            ostringstream ss;
            ss << differenceHisto->Integral(minIntBin, maxIntBin);
            string integral = ss.str();

            TLatex latex;
            latex.SetNDC();
            latex.SetTextSize(0.05);
            latex.SetTextAlign(13); // align at top
            latex.SetTextColor(kRed);

            string diffInt = "Integrated diff: ";
            latex.DrawLatex(0.20,0.85,diffInt.c_str());
            latex.DrawLatex(0.20, 0.78, integral.c_str());

            double yAxisLowEdge = targetHisto->GetMinimum();
            double yAxisHighEdge = targetHisto->GetMaximum();

            TLine *gateLowLine = new TLine(SIX_M_INT_LIMIT_MIN,yAxisLowEdge,SIX_M_INT_LIMIT_MIN,yAxisHighEdge);
            TLine *gateHighLine = new TLine(SIX_M_INT_LIMIT_MAX,yAxisLowEdge,SIX_M_INT_LIMIT_MAX,yAxisHighEdge);
            gateLowLine->SetLineStyle(7);
            gateLowLine->SetLineWidth(3);
            gateLowLine->SetLineColor(kBlue);
            gateLowLine->Draw();

            gateHighLine->SetLineStyle(7);
            gateHighLine->SetLineWidth(3);
            gateHighLine->SetLineColor(kBlue);
            gateHighLine->Draw();
        }

        else
        {
            cout << "For Sn112, no 6M histos found; skipping." << endl;
        }
    }

    {
        // Third quadrant: 4M det, Sn124
        c->cd(3);

        string targetHistoName = "histo4MTotal";
        string blankHistoName = "blank4MTotal";
        string differenceHistoName = "diff4M";

        TH1* targetHisto = (TH1*)Sn124File->Get(targetHistoName.c_str());
        TH1* blankHisto = (TH1*)Sn124File->Get(blankHistoName.c_str());
        TH1* differenceHisto = (TH1*)Sn124File->Get(differenceHistoName.c_str());

        if(targetHisto && blankHisto && differenceHisto)
        {
            // Pad dimensions and margins
            //gPad->SetPad(0, 0.8, 1, 0);
            gPad->SetLeftMargin(0.12);
            gPad->SetRightMargin(0.10);
            gPad->SetTopMargin(0.12);
            gPad->SetBottomMargin(0.12);
            //gPad->SetTicky(2);

            // Set histo point and line characteristics
            targetHisto->SetLineColor(kBlack);
            targetHisto->SetLineWidth(2);
            targetHisto->SetLineStyle(0);

            blankHisto->SetLineColor(kGray+2);
            blankHisto->SetLineWidth(2);
            blankHisto->SetLineStyle(0);

            differenceHisto->SetLineColor(kRed);
            differenceHisto->SetLineWidth(2);
            differenceHisto->SetLineStyle(0);

            // X-axis parameters
            targetHisto->GetXaxis()->SetTitle("TOF (a.u.)");
            targetHisto->GetXaxis()->SetTitleSize(0.04);
            targetHisto->GetXaxis()->SetTitleFont(2);
            targetHisto->GetXaxis()->SetTitleOffset(1.4);
            targetHisto->GetXaxis()->CenterTitle();

            targetHisto->GetXaxis()->SetLabelOffset(0.01);
            targetHisto->GetXaxis()->SetLabelSize(0.03);
            targetHisto->GetXaxis()->SetLabelFont(2);

            targetHisto->GetXaxis()->SetNdivisions(10);
            targetHisto->GetXaxis()->SetTickLength(0.03);

            targetHisto->GetXaxis()->SetRangeUser(1220,1350);

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

            string title = angle + " deg, Sn124, 4M";
            targetHisto->SetTitle(title.c_str());

            targetHisto->SetFillColor(19);
            blankHisto->SetFillColor(29);

            targetHisto->Draw("hist");
            blankHisto->Draw("hist same");
            //differenceHisto->Draw("hist same");

            int minIntBin = differenceHisto->FindBin(FOUR_M_INT_LIMIT_MIN);
            int maxIntBin = differenceHisto->FindBin(FOUR_M_INT_LIMIT_MAX);

            ostringstream ss;
            ss << differenceHisto->Integral(minIntBin, maxIntBin);
            string integral = ss.str();

            TLatex latex;
            latex.SetNDC();
            latex.SetTextSize(0.05);
            latex.SetTextAlign(13); // align at top
            latex.SetTextColor(kRed);

            string diffInt = "Integrated diff: ";
            latex.DrawLatex(0.20,0.85,diffInt.c_str());
            latex.DrawLatex(0.20, 0.78, integral.c_str());

            double yAxisLowEdge = targetHisto->GetMinimum();
            double yAxisHighEdge = targetHisto->GetMaximum();

            TLine *gateLowLine = new TLine(FOUR_M_INT_LIMIT_MIN,yAxisLowEdge,FOUR_M_INT_LIMIT_MIN,yAxisHighEdge);
            TLine *gateHighLine = new TLine(FOUR_M_INT_LIMIT_MAX,yAxisLowEdge,FOUR_M_INT_LIMIT_MAX,yAxisHighEdge);
            gateLowLine->SetLineStyle(7);
            gateLowLine->SetLineWidth(3);
            gateLowLine->SetLineColor(kBlue);
            gateLowLine->Draw();

            gateHighLine->SetLineStyle(7);
            gateHighLine->SetLineWidth(3);
            gateHighLine->SetLineColor(kBlue);
            gateHighLine->Draw();
        }

        else
        {
            cout << "For Sn124, no 4M histos found; skipping." << endl;
        }
    }

    {
        // Fourth quadrant: 6M det, Sn124
        c->cd(4);

        string targetHistoName = "histo6MTotal";
        string blankHistoName = "blank6MTotal";
        string differenceHistoName = "diff6M";

        TH1* targetHisto = (TH1*)Sn124File->Get(targetHistoName.c_str());
        TH1* blankHisto = (TH1*)Sn124File->Get(blankHistoName.c_str());
        TH1* differenceHisto = (TH1*)Sn124File->Get(differenceHistoName.c_str());

        if(targetHisto && blankHisto && differenceHisto)
        {
            // Pad dimensions and margins
            //gPad->SetPad(0, 0.8, 1, 0);
            gPad->SetLeftMargin(0.12);
            gPad->SetRightMargin(0.10);
            gPad->SetTopMargin(0.12);
            gPad->SetBottomMargin(0.12);
            //gPad->SetTicky(2);

            // Set histo point and line characteristics
            targetHisto->SetLineColor(kBlack);
            targetHisto->SetLineWidth(2);
            targetHisto->SetLineStyle(0);

            blankHisto->SetLineColor(kGray+2);
            blankHisto->SetLineWidth(2);
            blankHisto->SetLineStyle(0);

            differenceHisto->SetLineColor(kRed);
            differenceHisto->SetLineWidth(2);
            differenceHisto->SetLineStyle(0);

            // X-axis parameters
            targetHisto->GetXaxis()->SetTitle("TOF (a.u.)");
            targetHisto->GetXaxis()->SetTitleSize(0.04);
            targetHisto->GetXaxis()->SetTitleFont(2);
            targetHisto->GetXaxis()->SetTitleOffset(1.4);
            targetHisto->GetXaxis()->CenterTitle();

            targetHisto->GetXaxis()->SetLabelOffset(0.01);
            targetHisto->GetXaxis()->SetLabelSize(0.03);
            targetHisto->GetXaxis()->SetLabelFont(2);

            targetHisto->GetXaxis()->SetNdivisions(10);
            targetHisto->GetXaxis()->SetTickLength(0.03);

            targetHisto->GetXaxis()->SetRangeUser(900,1020);

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

            string title = angle + " deg, Sn124, 6M";
            targetHisto->SetTitle(title.c_str());

            targetHisto->SetFillColor(19);
            blankHisto->SetFillColor(29);

            targetHisto->Draw("hist");
            blankHisto->Draw("hist same");
            //differenceHisto->Draw("hist same");

            int minIntBin = differenceHisto->FindBin(SIX_M_INT_LIMIT_MIN);
            int maxIntBin = differenceHisto->FindBin(SIX_M_INT_LIMIT_MAX);

            ostringstream ss;
            ss << differenceHisto->Integral(minIntBin, maxIntBin);
            string integral = ss.str();

            TLatex latex;
            latex.SetNDC();
            latex.SetTextSize(0.05);
            latex.SetTextAlign(13); // align at top
            latex.SetTextColor(kRed);

            string diffInt = "Integrated diff: ";
            latex.DrawLatex(0.20,0.85,diffInt.c_str());
            latex.DrawLatex(0.20, 0.78, integral.c_str());

            double yAxisLowEdge = targetHisto->GetMinimum();
            double yAxisHighEdge = targetHisto->GetMaximum();

            TLine *gateLowLine = new TLine(SIX_M_INT_LIMIT_MIN,yAxisLowEdge,SIX_M_INT_LIMIT_MIN,yAxisHighEdge);
            TLine *gateHighLine = new TLine(SIX_M_INT_LIMIT_MAX,yAxisLowEdge,SIX_M_INT_LIMIT_MAX,yAxisHighEdge);
            gateLowLine->SetLineStyle(7);
            gateLowLine->SetLineWidth(3);
            gateLowLine->SetLineColor(kBlue);
            gateLowLine->Draw();

            gateHighLine->SetLineStyle(7);
            gateHighLine->SetLineWidth(3);
            gateHighLine->SetLineColor(kBlue);
            gateHighLine->Draw();
        }

        else
        {
            cout << "For Sn124, no 6M histos found; skipping." << endl;
        }
    }
}
