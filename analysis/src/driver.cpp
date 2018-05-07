// ROOT library classes
#include "TFile.h"
#include "TTree.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../include/Config.h"

#include "../include/histos.h"
#include "../include/subtractBackground.h"
#include "../include/calculateCS.h"

using namespace std;

Config config;

int main(int argc, char** argv)
{
    int numberArgsExpected = 3;
    if(argc < numberArgsExpected)
    {
        cerr << "Error: expected " << to_string(numberArgsExpected)
            << " arguments (\"inputFileName\""
            << " and \"outputFileName\"), but found " << to_string(argc)
            << " arguments." << endl;
        return 1;
    }

    string experiment = argv[1];
    string recreateHistos = argv[2];

    config = Config(experiment);

    if(recreateHistos=="true")
    {
        for(auto& run : config.runs)
        {
            string inputFileName = "../rawData/" + experiment + "/run"
                + to_string(run.number) + ".root";

            ifstream file(inputFileName.c_str());
            if(!file.is_open())
            {
                cerr << "Error: could not open " << inputFileName
                    << ". Skipping run " << run.number << endl;
                continue;
            }

            string outputFileName = "../processedData/" + experiment + "/runs/"
                + to_string(run.number) + "/histos.root";

            // if directory for this run doesn't exist yet, create it
            struct stat st = {0};

            stringstream ss;
            ss << run.number;
            string dirName = "../processedData/" + config.experiment + "/runs/" + ss.str();
            if(stat(dirName.c_str(), &st) == -1)
            {
                mkdir(dirName.c_str(), 0700);
            }

            histos(inputFileName, outputFileName);
        }
    }

    subtractBackground();
    calculateCS();
}
