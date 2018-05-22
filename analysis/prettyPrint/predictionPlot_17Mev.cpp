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
    style->SetMarkerSize(2);

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

    // read in literature graphs
    TFile* litFile = new TFile(litFileName.c_str(),"READ");
    
    string graph1Name = "Sn120(n,n) [17 MeV]";
    string graph2Name = "Sn124, 4M";
    string graph3Name = "Sn112, 4M";
    string graph4Name = "Sn124, 6M";
    string graph5Name = "Sn112, 6M";
    string graph6Name = "SnNat, 4M";
    string graph7Name = "SnNat, 6M";

    TGraphErrors* graph1 = (TGraphErrors*)litFile->Get(graph1Name.c_str());
    TGraphErrors* graph2 = (TGraphErrors*)litFile->Get(graph2Name.c_str());
    TGraphErrors* graph3 = (TGraphErrors*)litFile->Get(graph3Name.c_str());
    TGraphErrors* graph4 = (TGraphErrors*)litFile->Get(graph4Name.c_str());
    TGraphErrors* graph5 = (TGraphErrors*)litFile->Get(graph5Name.c_str());
    TGraphErrors* graph6 = (TGraphErrors*)litFile->Get(graph6Name.c_str());
    TGraphErrors* graph7 = (TGraphErrors*)litFile->Get(graph7Name.c_str());

    if(
            !graph1 ||
            !graph2 ||
            !graph3 ||
            !graph4 ||
            !graph5 ||
            !graph6 ||
            !graph7
      )
    {
        cerr << "Error: failed to open a data graph. Exiting..." << endl;
        exit(1);
    }

    // create multigraph for holding all graphs
    TMultiGraph *mg = new TMultiGraph();

    mg->Add(graph1, "l");
    mg->Add(graph2, "p");
    mg->Add(graph3, "p");
    mg->Add(graph4, "p");
    mg->Add(graph5, "p");
    mg->Add(graph6, "p");
    mg->Add(graph7, "p");

    // Set graph point and line characteristics
    graph1->SetLineColor(kBlack);
    graph1->SetLineWidth(3);
    graph1->SetLineStyle(0);

    graph2->SetMarkerColor(kRed);
    graph2->SetMarkerStyle(29);

    graph3->SetMarkerColor(kBlue);
    graph3->SetMarkerStyle(29);

    graph4->SetMarkerColor(kRed);
    graph4->SetMarkerStyle(23);

    graph5->SetMarkerColor(kBlue);
    graph5->SetMarkerStyle(23);

    graph6->SetMarkerColor(kMagenta-7);
    graph6->SetMarkerStyle(29);

    graph7->SetMarkerColor(kMagenta-7);
    graph7->SetMarkerStyle(23);

    mg->Draw("a");

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
    TLegend *legend = new TLegend(0.65,0.60,0.95,0.95);
    //legend->SetHeader("data","C");
    legend->SetTextSize(0.03);
    legend->AddEntry(graph1,"{}^{120}Sn, lit (P. Gus, 1989)","l");
    legend->AddEntry(graph2,"{}^{124}Sn, exp (4M det)","p");
    legend->AddEntry(graph4,"{}^{124}Sn, exp (6M det)","p");
    legend->AddEntry(graph3,"{}^{112}Sn, exp (4M det)","p");
    legend->AddEntry(graph5,"{}^{112}Sn, exp (6M det)","p");
    legend->AddEntry(graph6,"{}^{nat}Sn, exp (4M det)","p");
    legend->AddEntry(graph7,"{}^{nat}Sn, exp (6M det)","p");

    legend->Draw();

    c->Update();
}
