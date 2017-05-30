// Reads the scaler file for a given run (with the run number of interest
// inputted as a parameter) and returns a ScalerData object so that scaler data
// can be used to calculate cross sections.

#include <iostream>
#include <fstream>
#include <string>

#include "../include/scalerData.h"

using namespace std;

ScalerData::ScalerData(string fileName)
{
    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cerr << "Error: while attempting to read scaler data, failed to open " << fileName << ". Exiting..." << endl;
        exit(1);
    }

    // read in scaler data
    string dummy, dummy2;

    while(file >> dummy >> dummy2)
    {
        if(dummy=="01")
        {
            tenHzClock = stoi(dummy2);
        }

        else if(dummy=="02")
        {
            BP0 = stoi(dummy2);
        }

        else if(dummy=="03")
        {
            BCI = stoi(dummy2);
        }

        else if(dummy=="10")
        {
            Det4mCFD = stoi(dummy2);
        }

        else if(dummy=="11")
        {
            Det6mCFD = stoi(dummy2);
        }

        else if(dummy=="12")
        {
            Det0DegreeCFD = stoi(dummy2);
        }

        else if(dummy=="13")
        {
            Det0DegreeCFD_100 = stoi(dummy2);
        }

        else if(dummy=="14")
        {
            DetCMonCFD = stoi(dummy2);
        }

        else if(dummy=="15")
        {
            DetCMonCFD_10 = stoi(dummy2);
        }

        else if(dummy=="22")
        {
            VetoClock = stoi(dummy2);
        }

        else if(dummy=="23")
        {
            sixtyHzClock = stoi(dummy2);
        }

        else if(dummy=="25")
        {
            triggers = stoi(dummy2);
        }

        else if(dummy=="27")
        {
            liveTriggers = stoi(dummy2);
        }
    }
}
