void literatureSn_11MeV()
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

    TFile* litFile = new TFile(litFileName.c_str(),"READ");
    
    string Sn116GraphName = "Sn116(n,n) [11 MeV]";
    string Sn118GraphName = "Sn118(n,n) [11 MeV]";
    string Sn120GraphName = "Sn120(n,n) [11 MeV]";
    string Sn122GraphName = "Sn122(n,n) [11 MeV]";
    string Sn124GraphName = "Sn124(n,n) [11 MeV]";

    string ExpSn112GraphName = "ExpSn112(n,n) [11 MeV]";
    string ExpSn124GraphName = "ExpSn124(n,n) [11 MeV]";

    TGraphErrors* Sn116Graph = (TGraphErrors*)litFile->Get(Sn116GraphName.c_str());
    TGraphErrors* Sn118Graph = (TGraphErrors*)litFile->Get(Sn118GraphName.c_str());
    TGraphErrors* Sn120Graph = (TGraphErrors*)litFile->Get(Sn120GraphName.c_str());
    TGraphErrors* Sn122Graph = (TGraphErrors*)litFile->Get(Sn122GraphName.c_str());
    TGraphErrors* Sn124Graph = (TGraphErrors*)litFile->Get(Sn124GraphName.c_str());

    TGraphErrors* ExpSn112Graph = (TGraphErrors*)litFile->Get(ExpSn112GraphName.c_str());
    TGraphErrors* ExpSn124Graph = (TGraphErrors*)litFile->Get(ExpSn124GraphName.c_str());

    // create multigraph for holding all graphs
    TMultiGraph *mg = new TMultiGraph();

    mg->Add(Sn124Graph);
    mg->Add(Sn120Graph);
    mg->Add(Sn122Graph);
    mg->Add(Sn118Graph);
    mg->Add(Sn116Graph);
    mg->Add(ExpSn124Graph);
    mg->Add(ExpSn112Graph);

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

    Sn124Graph->SetLineColor(kRed+4);
    Sn124Graph->SetLineWidth(5);
    Sn124Graph->SetLineStyle(0);
    Sn124Graph->SetMarkerColor(kRed+4);
    Sn124Graph->SetMarkerStyle(8);
    Sn124Graph->SetMarkerSize(2);

    ExpSn124Graph->SetLineColor(kBlue);
    ExpSn124Graph->SetLineWidth(5);
    ExpSn124Graph->SetLineStyle(0);
    ExpSn124Graph->SetMarkerColor(kBlue);
    ExpSn124Graph->SetMarkerStyle(33);
    ExpSn124Graph->SetMarkerSize(4);

    ExpSn112Graph->SetLineColor(kBlue-10);
    ExpSn112Graph->SetLineWidth(5);
    ExpSn112Graph->SetLineStyle(0);
    ExpSn112Graph->SetMarkerColor(kBlue-10);
    ExpSn112Graph->SetMarkerStyle(33);
    ExpSn112Graph->SetMarkerSize(4);

    // X-axis parameters
    Sn116Graph->GetXaxis()->SetTitle("Angle (degrees, CM)");
    Sn116Graph->GetXaxis()->SetTitleSize(0.05);
    Sn116Graph->GetXaxis()->SetTitleFont(2);
    Sn116Graph->GetXaxis()->SetTitleOffset(1.4);
    Sn116Graph->GetXaxis()->CenterTitle();

    Sn116Graph->GetXaxis()->SetLabelOffset(0.01);
    Sn116Graph->GetXaxis()->SetLabelSize(0.05);
    Sn116Graph->GetXaxis()->SetLabelFont(2);

    Sn116Graph->GetXaxis()->SetNdivisions(10);
    Sn116Graph->GetXaxis()->SetTickLength(0.03);

    // Y-axis parameters
    Sn116Graph->GetYaxis()->SetTitle("#frac{d#sigma}{d#Omega} (mB/sr)");
    Sn116Graph->GetYaxis()->SetTitleSize(0.06);
    Sn116Graph->GetYaxis()->SetTitleFont(2);
    Sn116Graph->GetYaxis()->SetTitleOffset(1.1);
    Sn116Graph->GetYaxis()->CenterTitle();

    Sn116Graph->GetYaxis()->SetLabelOffset(0.01);
    Sn116Graph->GetYaxis()->SetLabelSize(0.05);

    Sn116Graph->GetYaxis()->SetLabelFont(2);
    Sn116Graph->GetYaxis()->SetNdivisions(10);
    Sn116Graph->GetYaxis()->SetTickLength(0.02);

    mg->Draw("ap");
    //Sn116Graph->Draw("");
    //Sn124Graph->Draw("same");
    //Sn122Graph->Draw("same");
    //Sn120Graph->Draw("same");
    //Sn118Graph->Draw("same");
    //Sn116Graph->Draw("same");

    //ExpSn124Graph->Draw("same");
    //ExpSn112Graph->Draw("same");

    gPad->SetLogy(1);
    Sn116Graph->GetYaxis()->SetRangeUser(0,9);

    //TLatex latex;
    //latex.SetNDC();
    //latex.SetTextSize(0.05);
    //latex.SetTextAlign(13); // align at top
    //latex.DrawLatex(0.65,0.65,"Pb");
    //latex.DrawLatex(0.47,0.52,"Sn");
    //latex.DrawLatex(0.32,0.4,"C");

    // Define legend format and contents
    TLegend *legend = new TLegend(0.75,0.6,0.85,0.85);
    //legend->SetHeader("data","C");
    legend->SetTextSize(0.03);
    legend->AddEntry(Sn116Graph,"{}^{116}Sn","l");
    legend->AddEntry(Sn118Graph,"{}^{118}Sn","l");
    legend->AddEntry(Sn120Graph,"{}^{120}Sn","l");
    legend->AddEntry(Sn122Graph,"{}^{122}Sn","l");
    legend->AddEntry(Sn124Graph,"{}^{124}Sn","l");
    legend->AddEntry(ExpSn112Graph,"Exp {}^{112}Sn","l");
    legend->AddEntry(ExpSn124Graph,"Exp {}^{124}Sn","l");

    legend->Draw();

    c1->Update();
}
