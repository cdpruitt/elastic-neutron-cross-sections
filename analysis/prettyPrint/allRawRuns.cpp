//vector<string> detectors = {"4M", "6M", "CMON"};

void allRawRuns(string experiment)
{
    // read runs to be plotted
    string configFileName = "../../configuration/" + experiment + "/runConfig.txt";

    ifstream configFile(configFileName.c_str());
    if(!configFile.is_open())
    {
        cerr << "Failed to find run config data in " << configFileName << std::endl;
        return;
    }

    string str;
    vector<string> runs;

    while(getline(configFile,str))
    {
        // ignore comments in data file
        string delimiter = " ";
        string token = str.substr(0,str.find(delimiter));
        if(!atoi(token.c_str()))
        {
            // This line starts with a non-integer and is thus a comment; ignore
            continue;
        }

        // parse data lines into space-delimited tokens
        vector<string> tokens;
        istringstream iss(str);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        runs.push_back(tokens[0]);
    }

    const unsigned int CANVAS_WIDTH = 2;
    const unsigned int CANVAS_HEIGHT = 4;

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

    for(int i=0; i<runs.size(); i++)
    {
        TCanvas* c;

        if(i%CANVAS_HEIGHT==0)
        {
            c = new TCanvas(runs[i].c_str(),"",CANVAS_WIDTH*280,CANVAS_HEIGHT*200);
            c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);
        }

        for(int detector=0; detector<detectors.size(); detector++)
        {
            // read integration limits from config file
            string intLimitFileName = "../../configuration/" + experiment
                + "/targets/Sn124/integrationLimits.txt";
            ifstream intLimitFile(intLimitFileName.c_str());

            if(!intLimitFile.is_open())
            {
                cerr << "Error: failed to open integration limit file "
                    << intLimitFileName << endl;
            }

            vector<double> intLimitsLow;
            vector<double> intLimitsHigh;

            string str;

            while(getline(intLimitFile,str))
            {
                // ignore comments in data file
                string delimiter = " ";
                string token = str.substr(0,str.find(delimiter));

                // parse data lines into space-delimited tokens
                vector<string> tokens;
                istringstream iss(str);
                copy(istream_iterator<string>(iss),
                        istream_iterator<string>(),
                        back_inserter(tokens));

                if(tokens.size()==0)
                {
                    continue;
                }

                if(tokens[0] == "Low")
                {
                    intLimitsLow.push_back(stod(tokens[3]));
                }

                if(tokens[0] == "High")
                {
                    intLimitsHigh.push_back(stod(tokens[3]));
                }
            }

            string fileName = "../../processedData/" + experiment + "/runs/" + runs[i] + "/histos.root";
            TFile* file = new TFile(fileName.c_str(),"READ");

            if(!file->IsOpen())
            {
                cerr << "Error: couldn't open " << fileName << endl;
                exit(1);
            }

            {
                c->cd(detector+1+CANVAS_WIDTH*(i%CANVAS_HEIGHT));

                string TDCName = detectors[detector] + "TDC";
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

                    string title = "Run " + runs[i] + ", " + detectors[detector] + " TDC";
                    TDC->SetTitle(title.c_str());

                    TDC->Draw("hist");

                    TDC->GetXaxis()->SetRangeUser(intLimitsLow[detector]-100,intLimitsHigh[detector]+100);

                    int minIntBin = TDC->FindBin(intLimitsLow[detector]);
                    int maxIntBin = TDC->FindBin(intLimitsHigh[detector]);

                    ostringstream ss;

                    double yAxisLowEdge = TDC->GetMinimum();
                    double yAxisHighEdge = TDC->GetMaximum();

                    TLine *gateLowLine = new TLine(intLimitsLow[detector],yAxisLowEdge,intLimitsLow[detector],yAxisHighEdge);
                    TLine *gateHighLine = new TLine(intLimitsHigh[detector],yAxisLowEdge,intLimitsHigh[detector],yAxisHighEdge);
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

            string name = "../plots/runs/" + to_string(i/CANVAS_HEIGHT) + ".png";
            img->WriteImage(name.c_str());

            delete c;
        }
    }
}
