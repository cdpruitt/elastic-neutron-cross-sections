// Plots detector efficiencies
// Ron Malone
// June 2016
// 

/**************************************************************/
// Change this filename!

TString fname = "./six-meter-eff/six-meter-eff.bias1xCs";
//  TString fname = "./four-meter-eff/four-meter-eff.bias2xCs";

/**************************************************************/

// Reads in vector for files with energy, eff data
// Skips lines with '#' at beginning
void read(string fname, vector<float>& en, vector<float>& eff) {

  ifstream infile;
  string dummyString;
  float enRead, effRead, errRead;
  int npoints;
  char first;

  infile.open( fname.c_str() );
  
  if (infile.is_open()) {

    // Read in S and cross section and store in arrays
    while (1) {

      first = infile.peek();

      // Skip lines with hash
      if (first == '#')
	getline(infile,dummyString);
      else
	infile >> enRead >> effRead;

      en.push_back(enRead);
      eff.push_back(effRead);
      
      if (infile.eof()) {
	break;
      }
    }
    
  }
  infile.close();
}

void graph_neff() {

  vector<float> en, eff;

  // Read in vectors for data
  read(fname.Data(),en,eff);

  TGraph *geff = new TGraph(en.size(), &(en[0]), &(eff[0]) );

  geff->SetLineColor(kBlue);
  geff->SetTitle("");
  geff->GetXaxis()->SetTitle("Neutron Energy (MeV)");
  geff->GetYaxis()->SetTitle("Efficiency");
  geff->GetXaxis()->SetTitleOffset(1.2);
  geff->GetYaxis()->SetTitleOffset(1.6);
  geff->GetXaxis()->CenterTitle();
  geff->GetYaxis()->CenterTitle();
  geff->GetXaxis()->SetRangeUser(0,20);

  TCanvas *c1 = new TCanvas("c1","c1",50,50,800,800);
  gStyle->SetOptStat(0);  

  c1->cd();
  gPad->SetLeftMargin(0.15);   // Careful - this is an old command (ROOT Version 5.26)
  gPad->SetBottomMargin(0.13);

  c1->Update();
  geff->Draw("A L");
}
