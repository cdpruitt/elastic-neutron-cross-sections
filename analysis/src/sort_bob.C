#include "../include/experimentalConstants.h"

struct data
{
    float PH; //pulse height
    float PSD; //pulse shape signal
    float TDC; // time
};

TFile fileTree("root_data/snmay_103_tree.root");
TTree * tree = (TTree*) fileTree.Get("tree");

int Nevents = tree->GetEntries();
cout << "Nevents=" << Nevents << endl;

//zero degree detector
data zdeg;

// 4meter detector
data four;

//six meter detector
data six;

// ceiling monitor
data ceil_mon;

//  main timing signal
float time;

tree->SetBranchAddress("0DEGPH",&zdeg.PH);
tree->SetBranchAddress("0DEGPSD",&zdeg.PSD);
tree->SetBranchAddress("0DEGTDC",&zdeg.TDC);

tree->SetBranchAddress("CMONPH",&ceil_mon.PH);
tree->SetBranchAddress("CMONPSD",&ceil_mon.PSD);
tree->SetBranchAddress("CMONTDC",&ceil_mon.TDC);

tree->SetBranchAddress("4MPH",&four.PH);
tree->SetBranchAddress("4MPSD",&four.PSD);
tree->SetBranchAddress("4MTDC",&four.TDC);

tree->SetBranchAddress("6MPH",&six.PH);
tree->SetBranchAddress("6MPSD",&six.PSD);
tree->SetBranchAddress("6MTDC",&six.TDC);

tree->SetBranchAddress("RFTDC",&time);

TFile fileOut("out.root","RECREATE");
TH1I*   hist_self_time_six = new TH1I("self_time_six","",1000,0,1000);
TH1I*   hist_self_time_four = new TH1I("self_time_four","",1000,0,1000);
TH1I*   hist_six_PSD = new TH1I("six_PSD","",2000,600,2600);
TH1I*   hist_four_PSD = new TH1I("four_PSD","",2000,600,2600);

TH1I*   hist_six_time = new TH1I("six_time","",151,900,1050);
TH1I*   hist_four_time = new TH1I("four_time","",151,1225,1375);
fileTree.cd();

for (int i=0;i<Nevents;i++)
{
    tree->GetEvent(i);

    hist_self_time_six->Fill(six.TDC);
    hist_self_time_four->Fill(four.TDC);

    if (six.TDC > 550 & six.TDC < 600)   //self timing gate
    {
        if (six.PH > SIX_METER_PULSE_HEIGHT_THRESHOLD)  //pulse heigh threshold
        {
            hist_six_PSD->Fill(six.PSD);

            if (six.PSD > SIX_METER_PSD_THRESHOLD) 
            {
                hist_six_time->Fill(time);
            }
        }
    }

    if (four.TDC > 550 & four.TDC < 600)   //self timing gate
    {
        if (four.PH > FOUR_METER_PULSE_HEIGHT_THRESHOLD)  //pulse heigh threshold
        {
            hist_four_PSD->Fill(four.PSD);

            if (four.PSD > FOUR_METER_PSD_THRESHOLD) 
            {
                hist_four_time->Fill(time);
            }
        }
    }
}

fileOut.Write();
