const double FOURM_INT_LIMIT_MIN = 1285;
const double FOURM_INT_LIMIT_MAX = 1315;

const double SIXM_INT_LIMIT_MIN = 940;
const double SIXM_INT_LIMIT_MAX = 985;

struct DiffGroup
{
    int groupNumber;
    double angle;
    int blank;
    int target1;
    int target2;
};

DiffGroup getDiffGroup(int groupNumber)
{
    ifstream configFile("../configuration/diffHistoConfig.txt");

    if(!configFile.is_open())
    {
        cerr << "Failed to find diff histo config file at configuration/diffHistoConfig.txt" << std::endl;
        exit(1);
    }

    string str;

    vector<DiffGroup> allGroups;

    while(getline(configFile,str))
    {
        // ignore comments in data file
        string delimiter = "-";
        string token = str.substr(0,str.find(delimiter));
        if(!atoi(token.c_str()))
        {
            // This line starts with a non-integer and is thus a comment; ignore
            continue;
        }

        DiffGroup group;
        stringstream(str) >> group.groupNumber >> group.angle >> group.blank >> group.target1 >> group.target2;

        if(groupNumber==group.groupNumber)
        {
            return group;
        }
    }

    return DiffGroup();
}

void plotDiffHistos(int groupNumber)
{
    const unsigned int CANVAS_WIDTH = 2;
    const unsigned int CANVAS_HEIGHT = 2;

    DiffGroup g = getDiffGroup(groupNumber);
    ostringstream ss;
    ss << fixed << setprecision(1) << g.angle;

    string angle = ss.str();
    
    //const unsigned int CANVAS_HEIGHT = ceil((double)expFileNames.size()/CANVAS_WIDTH);

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

    TCanvas* c = new TCanvas("c1","",CANVAS_WIDTH*300,CANVAS_HEIGHT*200);

    c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);

    string expFileName = "../../analyzedData/diffHistos/group" + to_string(groupNumber) + ".root";
    TFile* expFile = new TFile(expFileName.c_str(),"READ");

    if(!expFile)
    {
        cerr << "Error: couldn't open " << expFileName << endl;
        exit(1);
    }

    {
        // First quadrant: 4M det, 112Sn
        c->cd(1);

        string targetHistoName = "Sn112_4M";
        string blankHistoName = "blank_4M";
        string differenceHistoName = "Sn112Difference_4M";

        TH1I* targetHisto = (TH1I*)expFile->Get(targetHistoName.c_str());
        TH1I* blankHisto = (TH1I*)expFile->Get(blankHistoName.c_str());
        TH1I* differenceHisto = (TH1I*)expFile->Get(differenceHistoName.c_str());

        if(!targetHisto || !blankHisto || !differenceHisto)
        {
            cerr << "Error: couldn't find a histo needed for diff Histo plotting" << endl;
            exit(1);
        }

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

        blankHisto->SetLineColor(kGray);
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

        targetHisto->GetXaxis()->SetRangeUser(1260,1330);

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

        targetHisto->Draw("hist");
        blankHisto->Draw("hist same");
        differenceHisto->Draw("hist same");

        int minIntBin = differenceHisto->FindBin(FOURM_INT_LIMIT_MIN);
        int maxIntBin = differenceHisto->FindBin(FOURM_INT_LIMIT_MAX);

        ostringstream ss;
        ss << differenceHisto->Integral(minIntBin, maxIntBin);
        string integral = ss.str();

        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.05);
        latex.SetTextAlign(13); // align at top
        latex.SetTextColor(kRed);

        string diffInt = "Diff sum: ";
        latex.DrawLatex(0.20,0.65,diffInt.c_str());
        latex.DrawLatex(0.20, 0.58, integral.c_str());

        TLine *gateLowLine = new TLine(FOURM_INT_LIMIT_MIN,0,FOURM_INT_LIMIT_MIN,200);
        TLine *gateHighLine = new TLine(FOURM_INT_LIMIT_MAX,0,FOURM_INT_LIMIT_MAX,200);
        gateLowLine->SetLineStyle(7);
        gateLowLine->SetLineWidth(3);
        gateLowLine->SetLineColor(kBlue);
        gateLowLine->Draw();

        gateHighLine->SetLineStyle(7);
        gateHighLine->SetLineWidth(3);
        gateHighLine->SetLineColor(kBlue);
        gateHighLine->Draw();
    }

    {
        // Second quadrant: 6M det, 112Sn
        c->cd(2);

        string targetHistoName = "Sn112_6M";
        string blankHistoName = "blank_6M";
        string differenceHistoName = "Sn112Difference_6M";

        TH1I* targetHisto = (TH1I*)expFile->Get(targetHistoName.c_str());
        TH1I* blankHisto = (TH1I*)expFile->Get(blankHistoName.c_str());
        TH1I* differenceHisto = (TH1I*)expFile->Get(differenceHistoName.c_str());

        if(!targetHisto || !blankHisto || !differenceHisto)
        {
            cerr << "Error: couldn't find a histo needed for diff Histo plotting" << endl;
            exit(1);
        }

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

        blankHisto->SetLineColor(kGray);
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

        targetHisto->GetXaxis()->SetRangeUser(900,1000);

        // Y-axis parameters
        targetHisto->GetYaxis()->SetTitle("Counts");
        targetHisto->GetYaxis()->SetTitleSize(0.04);
        targetHisto->GetYaxis()->SetTitleFont(2);
        targetHisto->GetYaxis()->SetTitleOffset(1.3);
        targetHisto->GetYaxis()->CenterTitle();

        //targetHisto->GetYaxis()->SetLabelOffset(0.01);
        targetHisto->GetYaxis()->SetLabelSize(0.03);

        //targetHisto->GetYaxis()->SetLabelFont(2);
        //targetHisto->GetYaxis()->SetNdivisions(10);
        targetHisto->GetYaxis()->SetTickLength(0);

        string title = angle + " deg, Sn112, 6M";
        targetHisto->SetTitle(title.c_str());

        targetHisto->Draw("hist");
        blankHisto->Draw("hist same");
        differenceHisto->Draw("hist same");

        int minIntBin = differenceHisto->FindBin(SIXM_INT_LIMIT_MIN);
        int maxIntBin = differenceHisto->FindBin(SIXM_INT_LIMIT_MAX);

        ostringstream ss;
        ss << differenceHisto->Integral(minIntBin, maxIntBin);
        string integral = ss.str();

        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.05);
        latex.SetTextAlign(13); // align at top
        latex.SetTextColor(kRed);

        string diffInt = "Diff sum: ";
        latex.DrawLatex(0.20,0.65,diffInt.c_str());
        latex.DrawLatex(0.20, 0.58, integral.c_str());

        TLine *gateLowLine = new TLine(SIXM_INT_LIMIT_MIN,0,SIXM_INT_LIMIT_MIN,200);
        TLine *gateHighLine = new TLine(SIXM_INT_LIMIT_MAX,0,SIXM_INT_LIMIT_MAX,200);
        gateLowLine->SetLineStyle(7);
        gateLowLine->SetLineWidth(3);
        gateLowLine->SetLineColor(kBlue);
        gateLowLine->Draw();

        gateHighLine->SetLineStyle(7);
        gateHighLine->SetLineWidth(3);
        gateHighLine->SetLineColor(kBlue);
        gateHighLine->Draw();
    }

    {
        // Third quadrant: 4M det, 124Sn
        c->cd(3);

        string targetHistoName = "Sn124_4M";
        string blankHistoName = "blank_4M";
        string differenceHistoName = "Sn124Difference_4M";

        TH1I* targetHisto = (TH1I*)expFile->Get(targetHistoName.c_str());
        TH1I* blankHisto = (TH1I*)expFile->Get(blankHistoName.c_str());
        TH1I* differenceHisto = (TH1I*)expFile->Get(differenceHistoName.c_str());

        if(!targetHisto || !blankHisto || !differenceHisto)
        {
            cerr << "Error: couldn't find a histo needed for diff Histo plotting" << endl;
            exit(1);
        }

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

        blankHisto->SetLineColor(kGray);
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
        //targetHisto->GetXaxis()->SetTickLength(0.03);

        targetHisto->GetXaxis()->SetRangeUser(1260,1330);

        // Y-axis parameters
        targetHisto->GetYaxis()->SetTitle("Counts");
        targetHisto->GetYaxis()->SetTitleSize(0.04);
        targetHisto->GetYaxis()->SetTitleFont(2);
        targetHisto->GetYaxis()->SetTitleOffset(1.3);
        targetHisto->GetYaxis()->CenterTitle();

        //targetHisto->GetYaxis()->SetLabelOffset(0.01);
        targetHisto->GetYaxis()->SetLabelSize(0.03);

        //targetHisto->GetYaxis()->SetLabelFont(2);
        //targetHisto->GetYaxis()->SetNdivisions(10);
        targetHisto->GetYaxis()->SetTickLength(0);

        string title = angle + " deg, Sn124, 4M";
        targetHisto->SetTitle(title.c_str());

        targetHisto->Draw("hist");
        blankHisto->Draw("hist same");
        differenceHisto->Draw("hist same");

        int minIntBin = differenceHisto->FindBin(FOURM_INT_LIMIT_MIN);
        int maxIntBin = differenceHisto->FindBin(FOURM_INT_LIMIT_MAX);

        ostringstream ss;
        ss << differenceHisto->Integral(minIntBin, maxIntBin);
        string integral = ss.str();

        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.05);
        latex.SetTextAlign(13); // align at top
        latex.SetTextColor(kRed);

        string diffInt = "Diff sum: ";
        latex.DrawLatex(0.20,0.65,diffInt.c_str());
        latex.DrawLatex(0.20, 0.58, integral.c_str());

        TLine *gateLowLine = new TLine(FOURM_INT_LIMIT_MIN,0,FOURM_INT_LIMIT_MIN,200);
        TLine *gateHighLine = new TLine(FOURM_INT_LIMIT_MAX,0,FOURM_INT_LIMIT_MAX,200);
        gateLowLine->SetLineStyle(7);
        gateLowLine->SetLineWidth(3);
        gateLowLine->SetLineColor(kBlue);
        gateLowLine->Draw();

        gateHighLine->SetLineStyle(7);
        gateHighLine->SetLineWidth(3);
        gateHighLine->SetLineColor(kBlue);
        gateHighLine->Draw();
    }

    {
        // Fourth quadrant: 6M det, 124Sn
        c->cd(4);

        string targetHistoName = "Sn124_6M";
        string blankHistoName = "blank_6M";
        string differenceHistoName = "Sn124Difference_6M";

        TH1I* targetHisto = (TH1I*)expFile->Get(targetHistoName.c_str());
        TH1I* blankHisto = (TH1I*)expFile->Get(blankHistoName.c_str());
        TH1I* differenceHisto = (TH1I*)expFile->Get(differenceHistoName.c_str());

        if(!targetHisto || !blankHisto || !differenceHisto)
        {
            cerr << "Error: couldn't find a histo needed for diff Histo plotting" << endl;
            exit(1);
        }

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

        blankHisto->SetLineColor(kGray);
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
        //targetHisto->GetXaxis()->SetTickLength(0.03);

        targetHisto->GetXaxis()->SetRangeUser(900,1000);

        // Y-axis parameters
        targetHisto->GetYaxis()->SetTitle("Counts");
        targetHisto->GetYaxis()->SetTitleSize(0.04);
        targetHisto->GetYaxis()->SetTitleFont(2);
        targetHisto->GetYaxis()->SetTitleOffset(1.3);
        targetHisto->GetYaxis()->CenterTitle();

        //targetHisto->GetYaxis()->SetLabelOffset(0.01);
        targetHisto->GetYaxis()->SetLabelSize(0.03);

        //targetHisto->GetYaxis()->SetLabelFont(2);
        //targetHisto->GetYaxis()->SetNdivisions(10);
        targetHisto->GetYaxis()->SetTickLength(0);

        string title = angle + " deg, Sn124, 6M";
        targetHisto->SetTitle(title.c_str());

        targetHisto->Draw("hist");
        blankHisto->Draw("hist same");
        differenceHisto->Draw("hist same");

        int minIntBin = differenceHisto->FindBin(SIXM_INT_LIMIT_MIN);
        int maxIntBin = differenceHisto->FindBin(SIXM_INT_LIMIT_MAX);

        ostringstream ss;
        ss << differenceHisto->Integral(minIntBin, maxIntBin);
        string integral = ss.str();

        TLatex latex;
        latex.SetNDC();
        latex.SetTextSize(0.05);
        latex.SetTextAlign(13); // align at top
        latex.SetTextColor(kRed);

        string diffInt = "Diff sum: ";
        latex.DrawLatex(0.20,0.65,diffInt.c_str());
        latex.DrawLatex(0.20, 0.58, integral.c_str());

        TLine *gateLowLine = new TLine(SIXM_INT_LIMIT_MIN,0,SIXM_INT_LIMIT_MIN,200);
        TLine *gateHighLine = new TLine(SIXM_INT_LIMIT_MAX,0,SIXM_INT_LIMIT_MAX,200);
        gateLowLine->SetLineStyle(7);
        gateLowLine->SetLineWidth(3);
        gateLowLine->SetLineColor(kBlue);
        gateLowLine->Draw();

        gateHighLine->SetLineStyle(7);
        gateHighLine->SetLineWidth(3);
        gateHighLine->SetLineColor(kBlue);
        gateHighLine->Draw();

    }

    // 4M line
    //TLine *gateLowLine = new TLine(1300,0,1300,1000);
    // 6M line
    //TLine *gateLowLine = new TLine(955,0,955,1000);
    //gateLowLine->SetLineStyle(7);
    //gateLowLine->SetLineWidth(3);
    //gateLowLine->SetLineColor(kBlue);
    //gateLowLine->Draw();

    // 4M line
    //TLine *gateHighLine = new TLine(1330,0,1330,1000);
    // 6M line
    //TLine *gateHighLine = new TLine(985,0,985,1000);
    //gateHighLine->SetLineStyle(7);
    //gateHighLine->SetLineWidth(3);
    //gateHighLine->SetLineColor(kBlue);
    //gateHighLine->Draw();

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
