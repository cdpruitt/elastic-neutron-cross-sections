diagnosticHistos (unsigned int targetRunNumber, unsigned blankRunNumber)
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

    gROOT->SetStyle("histoStyle");
    gROOT->ForceStyle();

    TCanvas* canvas = new TCanvas("c1","",1200,1200);

    // Pad dimensions and margins
    gPad->SetPad(0.005, 0.995, 0.995, 0.005);
    gPad->SetLeftMargin(0.15);
    gPad->SetRightMargin(0.01);
    gPad->SetTopMargin(0.03);
    gPad->SetBottomMargin(0.15);
    gPad->SetTicky(2);

    string targetFileName = "../processedData/runs/" + itos(targetRunNumber) + "/histos.root";
    string blankRunNumber = "../processedData/runs/" + itos(blankRunNumber) + "/histos.root";
    
    // read in target histos
    TFile* expFile = new TFile(expFileName.c_str(),"READ");

    string exp4MHistoName = "4MTDCScaled";
    string exp6MHistoName = "6MTDCScaled";

    TH1I* target4MHisto = (TH1I*)expFile->Get(exp4MGraphName.c_str());
    TH1I* target6MHisto = (TH1I*)expFile->Get(exp6MGraphName.c_str());
    TH1I* blank4MHisto = (TH1I*)expFile->Get(exp4MGraphName.c_str());
    TH1I* blank6MHisto = (TH1I*)expFile->Get(exp6MGraphName.c_str());

    canvas.Divide(2,1);

    // 4M histo
    canvas.cd(1);
    target4MHisto->Draw();
    blank4MHisto->Draw("same");

    // 6M histo
    canvas.cd(2);
    target6MHisto->Draw();
    blank6MHisto->Draw("same");


    
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
    legend->AddEntry(ExpSn112_4MGraph,"Exp {}^{112}Sn, 4 meters","p");
    legend->AddEntry(ExpSn112_6MGraph,"Exp {}^{112}Sn, 6 meters","p");
    legend->AddEntry(ExpSn124_4MGraph,"Exp {}^{124}Sn, 4 meters","p");
    legend->AddEntry(ExpSn124_6MGraph,"Exp {}^{124}Sn, 6 meters","p");

    legend->Draw();
}
