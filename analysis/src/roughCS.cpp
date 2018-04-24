#include "../include/DiffGroup.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "TFile.h"
#include "TH1.h"

using namespace std;

//const double REFERENCE_CS = 229.4; // p,n cross section in mb/sr - OLD VALUE from 2017 (for 32.5 degrees @ 11 MeV)
const double REFERENCE_CS = 470; // p,n cross section in mb/sr - for 23 degrees @ 17 MeV
const double REFERENCE_NUMBER_OF_ATOMS = 2.91e23; // of polyethylene target
const double Sn112_NUMBER_OF_ATOMS = 2.676e22;
const double Sn124_NUMBER_OF_ATOMS = 2.697e22;

const double REFERENCE_4M_COUNTS = 10638; // integral gates are 1251->1282
const double REFERENCE_6M_COUNTS = 7023; // integral gates are 901->944
double REFERENCE_MONITOR_COUNTS = 1363; // integral gates are 1623->1775

const double FOURM_INT_LIMIT_MIN = 1285;
const double FOURM_INT_LIMIT_MAX = 1315;

const double SIXM_INT_LIMIT_MIN = 940;
const double SIXM_INT_LIMIT_MAX = 985;

DiffGroup getDiffGroup(int groupNumber)
{
    ifstream configFile("configuration/diffHistoConfig.txt");

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

int main(int, char** argv)
{

    DiffGroup g = getDiffGroup(atoi(argv[1]));
    ostringstream ss;
    ss << fixed << setprecision(1) << g.angle;

    string angle = ss.str();

    string diffFileName = "../analyzedData/diffHistos/group" + to_string(g.groupNumber) + ".root";
    TFile* diffFile = new TFile(diffFileName.c_str(),"READ");

    string differenceHistoName = "Sn112Difference_4M";
    TH1I* differenceHisto = (TH1I*)diffFile->Get(differenceHistoName.c_str());

    int minIntBin = differenceHisto->FindBin(FOURM_INT_LIMIT_MIN);
    int maxIntBin = differenceHisto->FindBin(FOURM_INT_LIMIT_MAX);

    double difference = differenceHisto->Integral(minIntBin, maxIntBin);

    string target1MonitorFileName = "../analyzedData/runs/" + to_string(g.target1) + "/histos.root";
    TFile* target1MonitorFile = new TFile(target1MonitorFileName.c_str());
    TH1I* target1MonitorHisto = (TH1I*)target1MonitorFile->Get("CMONTDC");

    string target2MonitorFileName = "../analyzedData/runs/" + to_string(g.target2) + "/histos.root";
    TFile* target2MonitorFile = new TFile(target1MonitorFileName.c_str());
    TH1I* target2MonitorHisto = (TH1I*)target2MonitorFile->Get("CMONTDC");

    double target1MonitorValue = target1MonitorHisto->GetEntries();
    double target2MonitorValue = target2MonitorHisto->GetEntries();

    cout << "difference/reference 4 counts" << difference/REFERENCE_4M_COUNTS << endl;
    cout << "target1MonitorValue/reference mon counts" << target1MonitorValue/REFERENCE_MONITOR_COUNTS << endl;

    // calculate differential cross section in lab frame
    double target1_4M_CS = ((double)difference/REFERENCE_4M_COUNTS)*
        ((double)target1MonitorValue/REFERENCE_MONITOR_COUNTS)*
        (REFERENCE_NUMBER_OF_ATOMS/Sn112_NUMBER_OF_ATOMS)*
        REFERENCE_CS;

    double target1_6M_CS = ((double)difference/REFERENCE_6M_COUNTS)*
        ((double)target1MonitorValue/REFERENCE_MONITOR_COUNTS)*
        (REFERENCE_NUMBER_OF_ATOMS/Sn112_NUMBER_OF_ATOMS)*
        REFERENCE_CS;

    double target2_4M_CS = ((double)difference/REFERENCE_4M_COUNTS)*
        ((double)target2MonitorValue/REFERENCE_MONITOR_COUNTS)*
        (REFERENCE_NUMBER_OF_ATOMS/Sn124_NUMBER_OF_ATOMS)*
        REFERENCE_CS;

    double target2_6M_CS = ((double)difference/REFERENCE_6M_COUNTS)*
        ((double)target2MonitorValue/REFERENCE_MONITOR_COUNTS)*
        (REFERENCE_NUMBER_OF_ATOMS/Sn124_NUMBER_OF_ATOMS)*
        REFERENCE_CS;

    cout << "Sn112 cross section, 4M @ " << angle << " degree: " << target1_4M_CS << endl;
    cout << "Sn112 cross section, 6M @ " << angle << " degree: " << target1_6M_CS << endl;
    cout << "Sn124 cross section, 4M @ " << angle << " degree: " << target2_4M_CS << endl;
    cout << "Sn124 cross section, 6M @ " << angle << " degree: " << target2_6M_CS << endl;
}

