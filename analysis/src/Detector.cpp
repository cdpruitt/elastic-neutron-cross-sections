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

        if(tokens[0]=="Distance:")
        {
            distance = stod(tokens[1]);
        }

        else if(tokens[0]=="Bins")
        {
            binsPerNS = stod(tokens[4]);
        }

        else if(tokens[0]=="Bin")
        {
            binOffset = stod(tokens[3]);
        }

        else if(tokens[0]=="Use" && tokens[3]=="yes")
        {
            useForCS = true;
        }
    }

    string bananaFileName = "../configuration/" + experiment
        + "/detectors/" + name + "/banana.data";

    banana = PIDBanana(bananaFileName);
}

void Detector::createHistos()
{
    histos = DetectorHistos(name);
    return;
}

int Detector::getTDCBin(double neutronTOF)
{
    return binsPerNS*neutronTOF + binOffset;
}
