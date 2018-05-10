vector<string> detectors = {"4M", "6M"};
vector<string> targets = {"Sn112", "Sn124"};

void allDiffHistos(string experiment)
{
    // read angles to be plotted
    string configFileName = "../../configuration/" + experiment + "/runConfig.txt";

    ifstream configFile(configFileName.c_str());
    if(!configFile.is_open())
    {
        cerr << "Failed to find run config data in " << configFileName << std::endl;
        return;
    }

    string str;
    vector<string> angles;

    while(getline(configFile,str))
    {
        bool skip4M = false;
        bool skip6M = false;

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

        for(auto& angle : angles)
        {
            if(angle==tokens[2])
            {
                skip4M = true;
            }
        }

        if(!skip4M)
        {
            angles.push_back(tokens[2]);
        }

        for(auto& angle : angles)
        {
            if(angle==tokens[3])
            {
                skip6M = true;
            }
        }

        if(!skip6M)
        {
            angles.push_back(tokens[3]);
        }
    }

    sort(angles.begin(), angles.end());

    const unsigned int CANVAS_WIDTH = 3;
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

    for(int i=0; i<angles.size(); i++)
    {
        TFile* outputFile;
        TCanvas* c;

        if(i%CANVAS_WIDTH==0)
        {
            string outputFileName = "angleSet" + to_string(i) + ".root";
            outputFile = new TFile(outputFileName.c_str(), "RECREATE");

            c = new TCanvas(outputFileName.c_str(),"",CANVAS_WIDTH*280,CANVAS_HEIGHT*200);
            c->Divide(CANVAS_WIDTH, CANVAS_HEIGHT);
        }

        for(int detector=0; detector<detectors.size(); detector++)
        {
            for(int target=0; target<targets.size(); target++)
            {
                // read integration limits from config file
                string intLimitFileName = "../../configuration/" + experiment
                    + "/targets/" + targets[target] + "/integrationLimits.txt";
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

                double canvasOffset;

                if(detectors[detector]=="4M" && targets[target]=="Sn112")
                {
                    canvasOffset = 1;
                }

                if(detectors[detector]=="6M" && targets[target]=="Sn112")
                {
                    canvasOffset = 4;
                }

                if(detectors[detector]=="4M" && targets[target]=="Sn124")
                {
                    canvasOffset = 7;
                }

                if(detectors[detector]=="6M" && targets[target]=="Sn124")
                {
                    canvasOffset = 10;
                }

                string fileName = "../../processedData/" + experiment + "/angles/" + angles[i] + "/" + targets[target] + ".root";
                TFile* file = new TFile(fileName.c_str(),"READ");

                if(!file->IsOpen())
                {
                    cerr << "Error: couldn't open " << fileName << endl;
                    exit(1);
                }

                {
                    c->cd(canvasOffset+(i%CANVAS_WIDTH));

                    string targetHistoName = "histo" + detectors[detector] + "Total";
                    string blankHistoName = "blank" + detectors[detector] + "Total";
                    string differenceHistoName = "diff" + detectors[detector];

                    TH1I* targetHisto = (TH1I*)file->Get(targetHistoName.c_str());
                    TH1I* blankHisto = (TH1I*)file->Get(blankHistoName.c_str());
                    TH1I* differenceHisto = (TH1I*)file->Get(differenceHistoName.c_str());

                    if(targetHisto && blankHisto && differenceHisto)
                    {
                        // Pad dimensions and margins
                        //gPad->SetPad(0, 0.8, 1, 0);
                        gPad->SetLeftMargin(0.15);
                        gPad->SetRightMargin(0.0);
                        gPad->SetTopMargin(0.15);
                        gPad->SetBottomMargin(0.18);
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
                        targetHisto->GetXaxis()->SetTitleSize(0.06);
                        targetHisto->GetXaxis()->SetTitleFont(2);
                        targetHisto->GetXaxis()->SetTitleOffset(1.4);
                        targetHisto->GetXaxis()->CenterTitle();

                        targetHisto->GetXaxis()->SetLabelOffset(0.01);
                        targetHisto->GetXaxis()->SetLabelSize(0.05);
                        targetHisto->GetXaxis()->SetLabelFont(2);

                        targetHisto->GetXaxis()->SetNdivisions(10);
                        targetHisto->GetXaxis()->SetTickLength(0.03);

                        // Y-axis parameters
                        targetHisto->GetYaxis()->SetTitle("Counts");
                        targetHisto->GetYaxis()->SetTitleSize(0.06);
                        targetHisto->GetYaxis()->SetTitleFont(2);
                        targetHisto->GetYaxis()->SetTitleOffset(1.3);
                        targetHisto->GetYaxis()->CenterTitle();

                        targetHisto->GetYaxis()->SetLabelOffset(0.01);
                        targetHisto->GetYaxis()->SetLabelSize(0.05);

                        targetHisto->GetYaxis()->SetLabelFont(2);
                        targetHisto->GetYaxis()->SetNdivisions(10);
                        targetHisto->GetYaxis()->SetTickLength(0);

                        string title = angles[i] + " deg, " + targets[target] + " " + detectors[detector];
                        targetHisto->SetTitle(title.c_str());

                        //targetHisto->SetFillColor(19);
                        //blankHisto->SetFillColor(29);

                        targetHisto->Draw("hist");
                        blankHisto->Draw("hist same");
                        //differenceHisto->Draw("hist same");

                        targetHisto->GetXaxis()->SetRangeUser(intLimitsLow[detector]-100,intLimitsHigh[detector]+100);

                        int minIntBin = differenceHisto->FindBin(intLimitsLow[detector]);
                        int maxIntBin = differenceHisto->FindBin(intLimitsHigh[detector]);

                        ostringstream ss;
                        ss << differenceHisto->Integral(minIntBin, maxIntBin);
                        string integral = ss.str();

                        TLatex latex;
                        latex.SetNDC();
                        latex.SetTextSize(0.05);
                        latex.SetTextAlign(13); // align at top
                        latex.SetTextColor(kRed);

                        string diffInt = "Integrated diff: ";
                        latex.DrawLatex(0.25,0.75,diffInt.c_str());
                        latex.DrawLatex(0.25, 0.68, integral.c_str());

                        double yAxisLowEdge = targetHisto->GetMinimum();
                        double yAxisHighEdge = targetHisto->GetMaximum();

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

                    else
                    {
                        TLatex latex;
                        latex.SetNDC();
                        latex.SetTextSize(0.06);
                        latex.SetTextAlign(13); // align at top
                        latex.SetTextColor(kGray+2);

                        string title = "No data for " + angles[i] + " deg, " + targets[target] + " " + detectors[detector];
                        latex.DrawLatex(0.3,0.50,title.c_str());

                        cout << "For " << targets[target] << "at " << angles[i] << ", no " << detectors[detector] << " histos found; skipping." << endl;
                    }
                }
            }
        }

        if(i%CANVAS_WIDTH==(CANVAS_WIDTH-1))
        {
            c->Update();
            TImage *img = TImage::Create();

            img->FromPad(c);

            string name = "../plots/tiling" + to_string(i/CANVAS_WIDTH) + ".png";
            img->WriteImage(name.c_str());

            delete c;
        }
    }
}

/*
 // read detector integration limits
    string intLimitsLocation = "../configuration/" + experiment + "/integrationLimits.txt";
    ifstream intLimitsFile(intLimitsLocation);

    if(!intLimitsFile.is_open())
    {
        cerr << "Error: couldn't open integration limit file " << intLimitsLocation << endl;
        return;
    }

    while(getline(intLimitsFile,str))
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
            intLimits.low.push_back(stod(tokens[3]));
        }
        
        if(tokens[0] == "High")
        {
            intLimits.high.push_back(stod(tokens[3]));
        }
    }
*/
