const double FOUR_M_INT_LIMIT_MIN = 1240;
const double FOUR_M_INT_LIMIT_MAX = 1282;

const double SIX_M_INT_LIMIT_MIN = 895;
const double SIX_M_INT_LIMIT_MAX = 946;

vector<string> detectors = {"4M", "6M"};
vector<string> targets = {"Sn112", "Sn124"};

void referenceDiffHistos(string experiment, string referenceSetName)
{
    string fileName = "../../configuration/" + experiment + "/normalization/" + referenceSetName + ".root";
    TFile* file = new TFile(fileName.c_str(),"READ");

    if(!file->IsOpen())
    {
        cerr << "Error: couldn't open " << fileName << endl;
        exit(1);
    }

    double INT_LIMIT_MIN;
    double INT_LIMIT_MAX;

    const unsigned int CANVAS_WIDTH = 2;
    const unsigned int CANVAS_HEIGHT = 1;

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

    TCanvas* c = new TCanvas("c","c",CANVAS_WIDTH*560,CANVAS_HEIGHT*500);
    c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);

    for(int i=0; i<detectors.size(); i++)
    {
        if(i==0)
        {
            c->cd(1);
            INT_LIMIT_MIN = FOUR_M_INT_LIMIT_MIN;
            INT_LIMIT_MAX = FOUR_M_INT_LIMIT_MAX;
        }

        else if(i==1)
        {
            c->cd(2);
            INT_LIMIT_MIN = SIX_M_INT_LIMIT_MIN;
            INT_LIMIT_MAX = SIX_M_INT_LIMIT_MAX;
        }

        string polyMinusBlankName = "polyMinusBlank" + detectors[i];
        string graphiteMinusBlankName = "graphiteMinusBlank" + detectors[i];
        string polyMinusGraphiteName = "polyMinusBlank" + detectors[i];

        TH1I* polyMinusBlank = (TH1I*)file->Get(polyMinusBlankName.c_str());
        TH1I* graphiteMinusBlank = (TH1I*)file->Get(graphiteMinusBlankName.c_str());
        TH1I* polyMinusGraphite = (TH1I*)file->Get(polyMinusGraphiteName.c_str());

        if(polyMinusBlank && graphiteMinusBlank && polyMinusGraphite)
        {
            // Pad dimensions and margins
            //gPad->SetPad(0, 0.8, 1, 0);
            gPad->SetLeftMargin(0.18);
            gPad->SetRightMargin(0.0);
            gPad->SetTopMargin(0.15);
            gPad->SetBottomMargin(0.18);
            //gPad->SetTicky(2);

            // Set histo point and line characteristics
            polyMinusBlank->SetLineColor(kBlack);
            polyMinusBlank->SetLineWidth(2);
            polyMinusBlank->SetLineStyle(0);

            graphiteMinusBlank->SetLineColor(kGray+2);
            graphiteMinusBlank->SetLineWidth(2);
            graphiteMinusBlank->SetLineStyle(0);

            polyMinusGraphite->SetLineColor(kRed);
            polyMinusGraphite->SetLineWidth(2);
            polyMinusGraphite->SetLineStyle(0);

            // X-axis parameters
            polyMinusBlank->GetXaxis()->SetTitle("TOF (a.u.)");
            polyMinusBlank->GetXaxis()->SetTitleSize(0.06);
            polyMinusBlank->GetXaxis()->SetTitleFont(2);
            polyMinusBlank->GetXaxis()->SetTitleOffset(1.4);
            polyMinusBlank->GetXaxis()->CenterTitle();

            polyMinusBlank->GetXaxis()->SetLabelOffset(0.01);
            polyMinusBlank->GetXaxis()->SetLabelSize(0.05);
            polyMinusBlank->GetXaxis()->SetLabelFont(2);

            polyMinusBlank->GetXaxis()->SetNdivisions(10);
            polyMinusBlank->GetXaxis()->SetTickLength(0.03);

            // Y-axis parameters
            polyMinusBlank->GetYaxis()->SetTitle("Counts");
            polyMinusBlank->GetYaxis()->SetTitleSize(0.06);
            polyMinusBlank->GetYaxis()->SetTitleFont(2);
            polyMinusBlank->GetYaxis()->SetTitleOffset(1.5);
            polyMinusBlank->GetYaxis()->CenterTitle();

            polyMinusBlank->GetYaxis()->SetLabelOffset(0.01);
            polyMinusBlank->GetYaxis()->SetLabelSize(0.05);

            polyMinusBlank->GetYaxis()->SetLabelFont(2);
            polyMinusBlank->GetYaxis()->SetNdivisions(10);
            polyMinusBlank->GetYaxis()->SetTickLength(0);

            polyMinusBlank->SetTitle(detectors[i].c_str());

            //polyMinusBlank->SetFillColor(19);
            //graphiteMinusBlank->SetFillColor(29);

            polyMinusBlank->Draw("hist");
            graphiteMinusBlank->Draw("hist same");
            polyMinusGraphite->Draw("hist same");

            polyMinusBlank->GetXaxis()->SetRangeUser(INT_LIMIT_MIN-100,INT_LIMIT_MAX+100);

            int minIntBin = polyMinusGraphite->FindBin(INT_LIMIT_MIN);
            int maxIntBin = polyMinusGraphite->FindBin(INT_LIMIT_MAX);

            ostringstream ss;
            ss << polyMinusGraphite->Integral(minIntBin, maxIntBin);
            string integral = ss.str();

            TLatex latex;
            latex.SetNDC();
            latex.SetTextSize(0.05);
            latex.SetTextAlign(13); // align at top
            latex.SetTextColor(kRed);

            string diffInt = "Diff integral:";
            latex.DrawLatex(0.25,0.75,diffInt.c_str());
            latex.DrawLatex(0.25, 0.68, integral.c_str());

            double yAxisLowEdge = polyMinusBlank->GetMinimum();
            double yAxisHighEdge = polyMinusBlank->GetMaximum();

            TLine *gateLowLine = new TLine(INT_LIMIT_MIN,yAxisLowEdge,INT_LIMIT_MIN,yAxisHighEdge);
            TLine *gateHighLine = new TLine(INT_LIMIT_MAX,yAxisLowEdge,INT_LIMIT_MAX,yAxisHighEdge);
            gateLowLine->SetLineStyle(7);
            gateLowLine->SetLineWidth(3);
            gateLowLine->SetLineColor(kBlue);
            gateLowLine->Draw();

            gateHighLine->SetLineStyle(7);
            gateHighLine->SetLineWidth(3);
            gateHighLine->SetLineColor(kBlue);
            gateHighLine->Draw();

            c->Update();
        }

        else
        {
            cerr << "Error: failed to open required reference histograms." << endl;
            exit(1);
        }
    }
}

