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
#include "../include/calculateReference.h"
#include "../include/ReferenceCS.h"
#include "../include/plotHistos.h"

using namespace std;

Config config;

int main(int argc, char** argv)
{
    int numberArgsExpected = 4;
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
    string plotsOnly = argv[3];

    config = Config(experiment);

    if(recreateHistos=="true")
    {
        for(auto& run : config.runs)
        {
            if(!run.isProduction)
            {
                continue;
            }

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

    vector<vector<ReferenceCS>> references;

    for(auto& d : config.detectors)
    {
        if(!d.useForCS)
        {
            continue;
        }

        references.push_back(vector<ReferenceCS>());

        for(int i=0; i<config.references.size(); i++)
        {
            references.back().push_back(ReferenceCS(config.references[i]));
            calculateReference(config.experiment, references.back().back(), d);
        }
    }

    plotReference(references);

    vector<ReferenceCS> combinedRefs;

    for(auto& refs : references)
    {
        combinedRefs.push_back(combineReferences(refs));
    }

    /*for(auto& r : combinedRefs)
    {
        cout << "difference = " << r.difference << endl;
        cout << "cross section = " << r.crossSection << endl;
    }*/

    subtractBackground();
    calculateCS(combinedRefs);

    plotHistos();
    plotDiffs();

    return 0;
}
