{
    TStyle * style = (TStyle*)gROOT->FindObject("graphStyle");

    if(!style)      
    {
        style = new TStyle("graphStyle","graphStyle");
    }

    style->SetOptStat(0);
    style->SetOptTitle(0);    
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
    style->SetTitleSize(0.06,"xyz");
    style->SetTitleFillColor(10);
    style->SetTitleTextColor(kBlack);
    style->SetEndErrorSize(0);

    gROOT->SetStyle("graphStyle");
    gROOT->ForceStyle();

    TCanvas* c = new TCanvas("c1","",1200,1200);

    // Pad dimensions and margins
    gPad->SetPad(0.005, 0.995, 0.995, 0.005);
    gPad->SetLeftMargin(0.15);
    gPad->SetRightMargin(0.01);
    gPad->SetTopMargin(0.03);
    gPad->SetBottomMargin(0.15);
    gPad->SetTicky(2);

    string litFileName = "../literatureData/literatureData.root";
    string expSn112FileName = "../experimentalData/Sn112/crossSections.root";

    // read in literature graphs
    TFile* litFile = new TFile(litFileName.c_str(),"READ");
    
    string litSn116_11MeVGraphName = "Sn116(n,n) [11 MeV]";

    string litSn116_24MeVGraphName = "Sn116(n,n) [24 MeV]";

    string DOMSn112_17MeVGraphName = "Sn112(n,n) [17 MeV]";

    TGraphErrors* litSn116_11MeVGraph = (TGraphErrors*)litFile->Get(litSn116_11MeVGraphName.c_str());

    TGraphErrors* litSn116_24MeVGraph = (TGraphErrors*)litFile->Get(litSn116_24MeVGraphName.c_str());

    if(
            !litSn116_11MeVGraph ||
            !litSn116_24MeVGraph
      )
    {
        cerr << "Error: failed to open a literature data graph. Exiting..." << endl;
        exit(1);
    }

    TGraphErrors* DOMSn112_17MeVGraph = (TGraphErrors*)litFile->Get(DOMSn112_17MeVGraphName.c_str());

    if(
            !DOMSn112_17MeVGraph
      )
    {
        cerr << "Error: failed to open DOM graph. Exiting..." << endl;
        exit(1);
    }

    // read in experimental graphs
    TFile* expSn112File = new TFile(expSn112FileName.c_str(),"READ");

    string expSn112_GraphName = "Exp. Sn112";

    TGraphErrors* ExpSn112_6MGraph = (TGraphErrors*)expSn112File->Get(expSn112_GraphName.c_str());

    if(
            !ExpSn112_6MGraph
      )
    {
        cerr << "Error: failed to open experimental data graph. Exiting..." << endl;
        exit(1);
    }

    // create multigraph for holding all graphs
    TMultiGraph *mg = new TMultiGraph();

    mg->Add(litSn116_11MeVGraph);
    mg->Add(litSn116_24MeVGraph);
    mg->Add(DOMSn112_17MeVGraph);
    mg->Add(ExpSn112_6MGraph);

    // Set graph point and line characteristics
    litSn116_11MeVGraph->SetLineColor(kBlue-7);
    litSn116_11MeVGraph->SetLineWidth(2);
    litSn116_11MeVGraph->SetLineStyle(0);

    litSn116_24MeVGraph->SetLineColor(kBlue-9);
    litSn116_24MeVGraph->SetLineWidth(2);
    litSn116_24MeVGraph->SetLineStyle(3);

    DOMSn112_17MeVGraph->SetLineColor(kRed);
    DOMSn112_17MeVGraph->SetLineWidth(6);
    DOMSn112_17MeVGraph->SetLineStyle(3);

    ExpSn112_6MGraph->SetLineColor(kRed);
    ExpSn112_6MGraph->SetLineWidth(6);
    ExpSn112_6MGraph->SetLineStyle(0);

    mg->Draw("al");

    // X-axis parameters
    mg->GetXaxis()->SetTitle("Angle (degrees, CM)");
    mg->GetXaxis()->SetTitleSize(0.05);
    mg->GetXaxis()->SetTitleFont(2);
    mg->GetXaxis()->SetTitleOffset(1.4);
    mg->GetXaxis()->CenterTitle();

    mg->GetXaxis()->SetLabelOffset(0.01);
    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetXaxis()->SetLabelFont(2);

    mg->GetXaxis()->SetNdivisions(10);
    mg->GetXaxis()->SetTickLength(0.03);

    // Y-axis parameters
    mg->GetYaxis()->SetTitle("#frac{d#sigma}{d#Omega} (mb/sr)");
    mg->GetYaxis()->SetTitleSize(0.06);
    mg->GetYaxis()->SetTitleFont(2);
    mg->GetYaxis()->SetTitleOffset(1.1);
    mg->GetYaxis()->CenterTitle();

    mg->GetYaxis()->SetLabelOffset(0.01);
    mg->GetYaxis()->SetLabelSize(0.05);

    mg->GetYaxis()->SetLabelFont(2);
    mg->GetYaxis()->SetNdivisions(10);
    mg->GetYaxis()->SetTickLength(0.02);
    
    mg->GetYaxis()->SetRangeUser(1,10000);
    gPad->SetLogy(1);

    //TLatex latex;
    //latex.SetNDC();
    //latex.SetTextSize(0.05);
    //latex.SetTextAlign(13); // align at top
    //latex.DrawLatex(0.65,0.65,"Pb");
    //latex.DrawLatex(0.47,0.52,"Sn");
    //latex.DrawLatex(0.32,0.4,"C");

    // Define legend format and contents
    TLegend *legend = new TLegend(0.4,0.65,0.95,0.9);
    //legend->SetHeader("data","C");
    legend->SetTextSize(0.03);
    legend->AddEntry(ExpSn112_6MGraph,"Last year's {}^{112}Sn, 11 MeV","l");
    legend->AddEntry(DOMSn112_17MeVGraph,"DOM prediction, {}^{112}Sn, 17 MeV","l");
    legend->AddEntry(litSn116_11MeVGraph,"Rapaport {}^{116}Sn, 11 MeV","l");
    legend->AddEntry(litSn116_24MeVGraph,"Rapaport {}^{116}Sn, 24 MeV","l");

    legend->Draw();

    c->Update();
}
