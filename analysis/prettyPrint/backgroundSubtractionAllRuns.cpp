vector<unsigned int> getIntegralBounds(string integralBoundsFileName)
{
    ifstream file(integralBoundsFileName.c_str());
    if(!file.is_open())
    {
        cerr << "Failed to find integral bounds data in " << integralBoundsFileName << std::endl;
        exit(1);
    }

    vector<unsigned int> integralBounds;

    string dummy;
    getline(file,dummy);

    unsigned int lowBound;
    unsigned int highBound;

    file >> lowBound >> highBound;

    integralBounds.push_back(lowBound);
    integralBounds.push_back(highBound);

    return integralBounds;
}

int main()
{
    const vector<string> expFileNames = 
    {
    };

    const unsigned int CANVAS_WIDTH = 4;
    const unsigned int CANVAS_HEIGHT = ceil((double)expFileNames.size()/CANVAS_WIDTH);

    TStyle * style = (TStyle*)gROOT->FindObject("graphStyle");

    if(!style)      
    {
        style = new TStyle("graphStyle","graphStyle");
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
    style->SetTitleX(0.15);
    style->SetEndErrorSize(0);

    gROOT->SetStyle("graphStyle");
    gROOT->ForceStyle();

    TCanvas* c = new TCanvas("c1","",CANVAS_WIDTH*200,CANVAS_HEIGHT*300);

    c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);

    for(int i=0; i<CANVAS_WIDTH*CANVAS_HEIGHT; i++)
    {
        if(i>=expFileNames.size())
        {
            break;
        }

        string expFileName = "../../analyzedData/angles/" + expFileNames[i];
        TFile* expFile = new TFile(expFileName.c_str(),"READ");

        string targetHistoName = "target";
        string blankHistoName = "blank";
        string differenceHistoName = "difference";

        TH1I* targetHisto = (TH1I*)expFile->Get(targetHistoName.c_str());
        TH1I* blankHisto = (TH1I*)expFile->Get(blankHistoName.c_str());
        TH1I* differenceHisto = (TH1I*)expFile->Get(differenceHistoName.c_str());

        if(!targetHisto || !blankHisto || !differenceHisto)
        {
            continue;
        }

        // Set histo point and line characteristics
        targetHisto->SetLineColor(kBlack);
        targetHisto->SetLineWidth(2);
        targetHisto->SetLineStyle(0);

        blankHisto->SetLineColor(kGray);
        blankHisto->SetLineWidth(2);
        blankHisto->SetLineStyle(0);

        differenceHisto->SetLineColor(kRed);
        differenceHisto->SetLineWidth(2);
        differenceHisto->SetLineStyle(0);

        // X-axis parameters
        //targetHisto->GetXaxis()->SetTitle("TOF (a.u.)");
        //targetHisto->GetXaxis()->SetTitleSize(0.04);
        //targetHisto->GetXaxis()->SetTitleFont(2);
        //targetHisto->GetXaxis()->SetTitleOffset(1.4);
        //targetHisto->GetXaxis()->CenterTitle();

        //targetHisto->GetXaxis()->SetLabelOffset(0.01);
        //targetHisto->GetXaxis()->SetLabelSize(0);
        //targetHisto->GetXaxis()->SetLabelFont(2);

        //targetHisto->GetXaxis()->SetNdivisions(10);
        //targetHisto->GetXaxis()->SetTickLength(0.03);

        // 4M range
        targetHisto->GetXaxis()->SetRangeUser(1280,1340);
        // 6M range
        //targetHisto->GetXaxis()->SetRangeUser(935,1005);

        // Y-axis parameters
        //targetHisto->GetYaxis()->SetTitle("Counts");
        //targetHisto->GetYaxis()->SetTitleSize(0.04);
        //targetHisto->GetYaxis()->SetTitleFont(2);
        //targetHisto->GetYaxis()->SetTitleOffset(1.3);
        //targetHisto->GetYaxis()->CenterTitle();

        //targetHisto->GetYaxis()->SetLabelOffset(0.01);
        targetHisto->GetYaxis()->SetLabelSize(0);

        //targetHisto->GetYaxis()->SetLabelFont(2);
        //targetHisto->GetYaxis()->SetNdivisions(10);
        targetHisto->GetYaxis()->SetTickLength(0);

        targetHisto->SetTitle(expFileNames[i].c_str());

        c->cd(i+1);

        // Pad dimensions and margins
        //gPad->SetPad(0, 0.8, 1, 0);
        gPad->SetLeftMargin(0);
        gPad->SetRightMargin(0);
        gPad->SetTopMargin(0.15);
        //gPad->SetBottomMargin(0.15);
        //gPad->SetTicky(2);

        targetHisto->Draw("l");
        blankHisto->Draw("same");
        differenceHisto->Draw("same");

        // 4M line
        TLine *gateLowLine = new TLine(1300,0,1300,1000);
        // 6M line
        //TLine *gateLowLine = new TLine(955,0,955,1000);
        gateLowLine->SetLineStyle(7);
        gateLowLine->SetLineWidth(3);
        gateLowLine->SetLineColor(kBlue);
        gateLowLine->Draw();

        // 4M line
        TLine *gateHighLine = new TLine(1330,0,1330,1000);
        // 6M line
        //TLine *gateHighLine = new TLine(985,0,985,1000);
        gateHighLine->SetLineStyle(7);
        gateHighLine->SetLineWidth(3);
        gateHighLine->SetLineColor(kBlue);
        gateHighLine->Draw();

        /*TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.05);
        latex.SetTextAlign(13); // align at top

        latex.DrawLatex(0.5,1.2,expFileNames[i].c_str());
        */
    }

    //gPad->SetLogy(1);

    // Define legend format and contents
    //TLegend *legend = new TLegend(0.7,0.65,0.95,0.9);
    //legend->SetHeader("data","C");
    //legend->SetTextSize(0.03);
    //legend->AddEntry(Sn116Graph,"{}^{116}Sn","p");
    //legend->AddEntry(Sn118Graph,"{}^{118}Sn","p");
    //legend->AddEntry(Sn120Graph,"{}^{120}Sn","p");
    //legend->AddEntry(Sn122Graph,"{}^{122}Sn","p");
    //legend->AddEntry(Sn124Graph,"Lit {}^{124}Sn, Rapaport","p");
    //legend->AddEntry(ExpSn112_4MGraph,"Exp {}^{112}Sn, 4 meters","p");
    //legend->AddEntry(ExpSn112_6MGraph,"Exp {}^{112}Sn, 6 meters","p");
    //legend->AddEntry(ExpSn124_4MGraph,"Exp {}^{124}Sn, 4 meters","p");
    //legend->AddEntry(ExpSn124_6MGraph,"Exp {}^{124}Sn, 6 meters","p");

    //legend->Draw();

    c->Update();
}
