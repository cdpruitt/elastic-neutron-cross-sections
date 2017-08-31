// Reads the scaler file for a given run (with the run number of interest
// inputted as a parameter) and returns a ScalerData object so that scaler data
// can be used to calculate cross sections.

#include <iostream>
#include <fstream>
#include <string>

#include "../include/ScalerData.h"

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
        if(dummy=="1")
        {
            tenHzClock = stoull(dummy2);
        }

        if(dummy=="2")
        {
            BP0 = stoull(dummy2);
        }

        else if(dummy=="3")
        {
            BCI = stoull(dummy2);
        }

        else if(dummy=="10")
        {
            Det4mCFD = stoull(dummy2);
        }

        else if(dummy=="11")
        {
            Det6mCFD = stoull(dummy2);
        }

        else if(dummy=="12")
        {
            Det0DegreeCFD = stoull(dummy2);
        }

        else if(dummy=="13")
        {
            Det0DegreeCFD_100 = stoull(dummy2);
        }

        else if(dummy=="14")
        {
            DetCMonCFD = stoull(dummy2);
        }

        else if(dummy=="15")
        {
            DetCMonCFD_10 = stoull(dummy2);
        }

        else if(dummy=="22")
        {
            VetoClock = stoull(dummy2);
        }

        else if(dummy=="23")
        {
            sixtyHzClock = stoull(dummy2);
        }

        else if(dummy=="25")
        {
            triggers = stoull(dummy2);
        }

        else if(dummy=="27")
        {
            liveTriggers = stoull(dummy2);
        }
    }
}
