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
    string expFileName = "../experimentalData/experimentalData.root";

    // read in literature graphs
    TFile* litFile = new TFile(litFileName.c_str(),"READ");
    
    string Sn116GraphName = "Sn116(n,n) [11 MeV]";
    string Sn118GraphName = "Sn118(n,n) [11 MeV]";
    string Sn120GraphName = "Sn120(n,n) [11 MeV]";
    string Sn122GraphName = "Sn122(n,n) [11 MeV]";
    string Sn124GraphName = "Sn124(n,n) [11 MeV]";

    TGraphErrors* Sn116Graph = (TGraphErrors*)litFile->Get(Sn116GraphName.c_str());
    TGraphErrors* Sn118Graph = (TGraphErrors*)litFile->Get(Sn118GraphName.c_str());
    TGraphErrors* Sn120Graph = (TGraphErrors*)litFile->Get(Sn120GraphName.c_str());
    TGraphErrors* Sn122Graph = (TGraphErrors*)litFile->Get(Sn122GraphName.c_str());
    TGraphErrors* Sn124Graph = (TGraphErrors*)litFile->Get(Sn124GraphName.c_str());

    // read in experimental graphs
    TFile* expFile = new TFile(expFileName.c_str(),"READ");

    string expSn112_6MGraphName = "Sn112(n,n) [11 MeV, 6M]";
    string expSn124_6MGraphName = "Sn124(n,n) [11 MeV, 6M]";

    TGraphErrors* ExpSn112_6MGraph = (TGraphErrors*)expFile->Get(expSn112_6MGraphName.c_str());
    TGraphErrors* ExpSn124_6MGraph = (TGraphErrors*)expFile->Get(expSn124_6MGraphName.c_str());

    // create multigraph for holding all graphs
    TMultiGraph *mg = new TMultiGraph();

    mg->Add(Sn124Graph);
    //mg->Add(Sn120Graph);
    //mg->Add(Sn122Graph);
    //mg->Add(Sn118Graph);
    //mg->Add(Sn116Graph);
    mg->Add(ExpSn112_6MGraph);
    mg->Add(ExpSn124_6MGraph);

    // Set graph point and line characteristics
    Sn116Graph->SetLineColor(kRed-9);
    Sn116Graph->SetLineWidth(5);
    Sn116Graph->SetLineStyle(0);
    Sn116Graph->SetMarkerColor(kRed-9);
    Sn116Graph->SetMarkerStyle(8);
    Sn116Graph->SetMarkerSize(2);

    Sn118Graph->SetLineColor(kRed-5);
    Sn118Graph->SetLineWidth(5);
    Sn118Graph->SetLineStyle(0);
    Sn118Graph->SetMarkerColor(kRed-5);
    Sn118Graph->SetMarkerStyle(8);
    Sn118Graph->SetMarkerSize(2);

    Sn120Graph->SetLineColor(kRed);
    Sn120Graph->SetLineWidth(5);
    Sn120Graph->SetLineStyle(0);
    Sn120Graph->SetMarkerColor(kRed);
    Sn120Graph->SetMarkerStyle(8);
    Sn120Graph->SetMarkerSize(2);

    Sn122Graph->SetLineColor(kRed+2);
    Sn122Graph->SetLineWidth(5);
    Sn122Graph->SetLineStyle(0);
    Sn122Graph->SetMarkerColor(kRed+2);
    Sn122Graph->SetMarkerStyle(8);
    Sn122Graph->SetMarkerSize(2);

    Sn124Graph->SetLineColor(kBlack);
    Sn124Graph->SetLineWidth(1);
    Sn124Graph->SetLineStyle(1);
    Sn124Graph->SetMarkerColor(kBlack);
    Sn124Graph->SetMarkerStyle(8);
    Sn124Graph->SetMarkerSize(2);

    ExpSn112_6MGraph->SetLineColor(kRed-9);
    ExpSn112_6MGraph->SetLineWidth(1);
    ExpSn112_6MGraph->SetLineStyle(1);
    ExpSn112_6MGraph->SetMarkerColor(kRed-9);
    ExpSn112_6MGraph->SetMarkerStyle(29);
    ExpSn112_6MGraph->SetMarkerSize(4);

    ExpSn124_6MGraph->SetLineColor(kRed+1);
    ExpSn124_6MGraph->SetLineWidth(1);
    ExpSn124_6MGraph->SetLineStyle(1);
    ExpSn124_6MGraph->SetMarkerColor(kRed+1);
    ExpSn124_6MGraph->SetMarkerStyle(29);
    ExpSn124_6MGraph->SetMarkerSize(4);

    mg->Draw("apl");

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
    TLegend *legend = new TLegend(0.7,0.65,0.95,0.9);
    //legend->SetHeader("data","C");
    legend->SetTextSize(0.03);
    //legend->AddEntry(Sn116Graph,"{}^{116}Sn","p");
    //legend->AddEntry(Sn118Graph,"{}^{118}Sn","p");
    //legend->AddEntry(Sn120Graph,"{}^{120}Sn","p");
    //legend->AddEntry(Sn122Graph,"{}^{122}Sn","p");
    legend->AddEntry(Sn124Graph,"Lit {}^{124}Sn, Rapaport","p");
    legend->AddEntry(ExpSn112_6MGraph,"Exp {}^{112}Sn","p");
    legend->AddEntry(ExpSn124_6MGraph,"Exp {}^{124}Sn","p");

    legend->Draw();

    c->Update();
}
