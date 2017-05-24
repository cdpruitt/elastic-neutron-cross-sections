// For calculating elastic cross section data using raw experimental data and
// by scaling an existing literature data point

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

#include "../include/DataStructures.h"

using namespace std;

void readTargetDataFile(TargetData& targetData, string fileName)
{
    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cerr << "Error: failed to open " << fileName << ". Exiting..." << endl;
        exit(1);
    }

    //ignore two label lines
    string dummy, dummy2;
    getline(file, dummy);

    file >> dummy;
    targetData.numberOfAtoms = stod(dummy);

    file >> dummy2;
    if(dummy2!="")
    {
        targetData.ControlCS = stod(dummy2);
    }
}

void readRawDataFile(vector<RawDataPoint>& rawData, string fileName)
{
    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cerr << "Error: failed to open " << fileName << ". Exiting..." << endl;
        exit(1);
    }

    // ignore two label lines
    string dummy, dummy2, dummy3, dummy4, dummy5;
    getline(file, dummy);
    getline(file, dummy);

    while(file >> dummy >> dummy2 >> dummy3 >> dummy4 >> dummy5)
    {
        RawDataPoint newDP;

        newDP.angle = stod(dummy);

        if(dummy2=="-")
        {
            newDP.Det4M = 0;
        }

        else
        {
            newDP.Det4M = stoi(dummy2);
        }

        if(dummy3=="-")
        {
            newDP.Det6M = 0;
        }

        else
        {
            newDP.Det6M = stoi(dummy3);
        }

        newDP.Mon0Degree = stoi(dummy4);
        newDP.MonCeiling = stoi(dummy5);

        rawData.push_back(newDP);
    }
}

int main(int argc, char** argv)
{
    string targetName = argv[1];

    // open data files in preparation for calculation
    string rawDataFileName = "experimentalData/" + targetName + "/rawData.txt";
    string expTargetFileName = "experimentalData/" + targetName + "/targetData.txt";

    string controlFileName = "experimentalData/polyethylene/rawData.txt";
    string controlTargetFileName = "experimentalData/polyethylene/targetData.txt";

    // read raw data and target data for target of interest
    vector<RawDataPoint> rawData;
    readRawDataFile(rawData, rawDataFileName);

    TargetData expTargetData;
    readTargetDataFile(expTargetData, expTargetFileName);

    // read raw data and target data for control target
    vector<RawDataPoint> controlData;
    readRawDataFile(controlData, controlFileName);

    TargetData controlTargetData;
    readTargetDataFile(controlTargetData, controlTargetFileName);

    // open output file in preparation for recording calculated cross sections
    string outputFileName = "experimentalData/" + targetName + "/crossSections.txt";
    ofstream outputFile(outputFileName);

    // print output file header
    outputFile << "Experimental data from 2017 measurement at TUNL" << endl;
    outputFile  << "Exp. " << targetName << endl;
    outputFile << "Degrees     mB/sr     Error (mB/sr)" << endl;

    RawDataPoint cdp = controlData[0];

    // for each raw data point, calculate cross section by scaling the control
    for(RawDataPoint dp : rawData)
    {
        double crossSection = 0;

        if(dp.Det4M!=0)
        {
            crossSection += ((double)dp.Det4M/cdp.Det4M)*
                ((double)cdp.MonCeiling/dp.MonCeiling)*
                ((double)controlTargetData.numberOfAtoms/expTargetData.numberOfAtoms)*
                controlTargetData.ControlCS;
        }

        if(dp.Det6M!=0)
        {
            crossSection += ((double)dp.Det6M/cdp.Det6M)*
                ((double)cdp.MonCeiling/dp.MonCeiling)*
                ((double)controlTargetData.numberOfAtoms/expTargetData.numberOfAtoms)*
                controlTargetData.ControlCS;
        }

        // if cross section is available from both detectors, average them
        if(dp.Det4M!=0 && dp.Det6M!=0)
        {
            crossSection /= 2;
        }

        outputFile << dp.angle << "       " << crossSection << "    " << "0" << endl;
    }

    return 0;
}
