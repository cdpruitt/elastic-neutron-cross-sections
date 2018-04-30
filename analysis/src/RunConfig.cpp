#include "../include/RunConfig.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iomanip>

using namespace std;

// Determine the angle and target for a given run
AllConfigs getRunConfig()
{
    string targetOrderLocation = "configuration/runConfig.txt";

    ifstream dataFile(targetOrderLocation.c_str());
    if(!dataFile.is_open())
    {
        cerr << "Failed to find target order data in " << targetOrderLocation << std::endl;
        exit(1);
    }

    string str;

    AllConfigs allRuns;

    while(getline(dataFile,str))
    {
        // ignore comments in data file
        string delimiter = "-";
        string token = str.substr(0,str.find(delimiter));
        if(!atoi(token.c_str()))
        {
            // This line starts with a non-integer and is thus a comment; ignore
            continue;
        }

        // parse data lines into space-delimited tokens
        vector<string> tokens;
        istringstream iss(str);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        allRuns.runs.push_back(RunConfig(tokens));
    }

    return allRuns;
}

AngleData AllAngles::getAngle(double a)
{
    for(auto& angle : angles)
    {
        if(angle.angle==a)
        {
            return angle;
        }
    }

    return AngleData();
}

void AllAngles::getRunData(AllConfigs ac)
{
    for(auto& run : ac.runs)
    {
        bool alreadyAdded = false;

        for(auto& angle : angles)
        {
            if(angle.angle==run.angle4M)
            {
                alreadyAdded = true;
                break;
            }
        }

        if(!alreadyAdded)
        {
            AngleData a;
            a.angle = run.angle4M;
            angles.push_back(a);
        }
    }

    for(auto& run : ac.runs)
    {
        bool alreadyAdded = false;

        for(auto& angle : angles)
        {
            if(angle.angle==run.angle6M)
            {
                alreadyAdded = true;
                break;
            }
        }

        if(!alreadyAdded)
        {
            AngleData a;
            a.angle = run.angle6M;
            angles.push_back(a);
        }
    }
    
    sort(angles.begin(), angles.end());
}

RunConfig AllConfigs::getRunConfig(int runNumber)
{
    for(auto& run : runs)
    {
        if(run.runNumber==runNumber)
        {
            return run;
        }
    }

    return RunConfig();
}
