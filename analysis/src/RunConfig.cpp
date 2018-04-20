#include "../include/RunConfig.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <iomanip>

using namespace std;

// Determine the angle and target for a given run
vector<RunConfig> getRunConfig(string detectorName)
{
    string targetOrderLocation = "configuration/runConfig.txt";

    ifstream dataFile(targetOrderLocation.c_str());
    if(!dataFile.is_open())
    {
        cerr << "Failed to find target order data in " << targetOrderLocation << std::endl;
        exit(1);
    }

    string str;
    vector<RunConfig> allRuns;

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

        vector<string> singleDetConfig;

        singleDetConfig.push_back(tokens[0]);
        singleDetConfig.push_back(tokens[1]);

        if(detectorName=="4M")
        {
            singleDetConfig.push_back(tokens[2]);
        }

        else if(detectorName=="6M")
        {
            singleDetConfig.push_back(tokens[3]);
        }

        else
        {
            cerr << "Error: attempted to read run config, but detector named " << detectorName << " is not an implemented option." << endl;
            exit(1);
        }

        allRuns.push_back(singleDetConfig);
    }

    return allRuns;
}
