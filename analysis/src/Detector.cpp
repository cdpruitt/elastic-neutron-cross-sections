#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>

#include "../include/Detector.h"

using namespace std;

Detector::Detector(string n, string experiment) : name(n)
{
    string basicFileName = "../configuration/" + experiment
        + "/detectors/" + name + "/basic.txt";

    ifstream basicFile(basicFileName.c_str());
    if(!basicFile.is_open())
    {
        cerr << "Failed to find detector configuration file in " << basicFileName << std::endl;
        return;
    }

    string str;

    while(getline(basicFile,str))
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

        if(tokens[0]=="Distance")
        {
            distance = stod(tokens.back());
        }

        else if(tokens[0]=="Linear")
        {
            linearCalibration = stod(tokens.back());
        }

        else if(tokens[0]=="Time")
        {
            timeOffset = stod(tokens.back());
        }

        else if(tokens[0]=="Resolution")
        {
            resolution = stod(tokens.back());
        }

        else if(tokens[0]=="Reference")
        {
            refResolution = stod(tokens.back());
        }

        else if(tokens[0]=="Use" && tokens.back()=="yes")
        {
            useForCS = true;
        }
    }

    string bananaFileName = "../configuration/" + experiment
        + "/detectors/" + name + "/banana.data";

    banana = PIDBanana(bananaFileName);

    string efficiencyFileName = "../configuration/" + experiment
        + "/detectors/" + name + "/efficiency_1xCs.txt";

    efficiency = DetectorEfficiency(efficiencyFileName);
}

int Detector::getTDCBin(double neutronTOF)
{
    return (neutronTOF+timeOffset)/linearCalibration;
}
