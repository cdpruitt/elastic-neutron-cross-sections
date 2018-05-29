#include "../include/DetectorEfficiency.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

DetectorEfficiency::DetectorEfficiency(string fileName)
{
    ifstream efficiencyFile(fileName.c_str());
    if(!efficiencyFile.is_open())
    {
        cerr << "Error: couldn't open efficiency file " + fileName + "." << endl;
        return;
    }

    string line;

    while(getline(efficiencyFile, line))
    {
        vector<string> tokens;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        energy.push_back(stod(tokens[0]));
        efficiency.push_back(stod(tokens[1]));
    }
}

double DetectorEfficiency::getEfficiency(double e) const
{
    for(int i=0; i<energy.size(); i++)
    {
        if((energy[i] > e) && energy[i-1] < e)
        {
            double eff = (efficiency[i]+efficiency[i-1])/2;
            
            if(eff>1 || eff<0)
            {
                cerr << "Error: for " << to_string(e)
                    << " MeV, calculated an efficiency of " << eff << endl;
                exit(1);
            }
            
            return eff;
        }
    }

    return 1;
}
