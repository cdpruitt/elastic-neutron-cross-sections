#include "../include/Config.h"
#include "../include/experimentalConstants.h"
#include "../include/Detector.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

using namespace std;

// Read the angles and target for a given run
Config::Config(string experiment) : experiment(experiment)
{
    string fileName = "../configuration/" + experiment + "/runConfig.txt";

    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cerr << "Failed to find configuration file in " << fileName << std::endl;
        return;
    }

    string str;

    while(getline(file,str))
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

        runs.push_back(Run(tokens, experiment));
    }

    for(auto& run : runs)
    {
        if(!run.isProduction)
        {
            continue;
        }

        for(auto& runAngle : run.angles)
        {
            bool alreadyAdded = false;

            for(auto& angle : angles)
            {
                if(angle.value==runAngle)
                {
                    alreadyAdded = true;
                    break;
                }
            }

            if(!alreadyAdded)
            {
                Angle a;
                a.value = runAngle;
                angles.push_back(a);
            }
        }
    }

    sort(angles.begin(), angles.end());

    for(auto& angle : angles)
    {
        for(auto& run : runs)
        {
            for(auto& runAngle : run.angles)
            {
                if(angle.value==runAngle)
                {
                    bool alreadyAdded = false;

                    for(auto& target : angle.targets)
                    {
                        if(target.name==run.target.name)
                        {
                            alreadyAdded = true;
                            break;
                        }
                    }

                    if(!alreadyAdded)
                    {
                        angle.targets.push_back(run.target);
                    }
                }
            }
        }
    }

    for(auto& name : DETECTOR_NAMES)
    {
        detectors.push_back(Detector(name, experiment));

        for(auto& useForCSName : USE_FOR_CS_NAMES)
        {
            if(name==useForCSName)
            {
                detectors.back().useForCS = true;
                break;
            }
        }
    }

    string neutronEnergyFileName = "../configuration/" + experiment + "/neutronEnergy.txt";

    ifstream neutronEnergyFile(neutronEnergyFileName.c_str());
    if(!neutronEnergyFile.is_open())
    {
        cerr << "Failed to find configuration file in " << neutronEnergyFileName << std::endl;
        return;
    }

    neutronEnergyFile >> neutronEnergy;

    string referenceFileName = "../configuration/" + experiment + "/reference.txt";

    ifstream referenceFile(referenceFileName.c_str());
    if(!referenceFile.is_open())
    {
        cerr << "Failed to find reference CS file at " << referenceFileName << std::endl;
        return;
    }

    referenceFile >> reference;
}

Run Config::getRun(int runNumber)
{
    for(auto& run : runs)
    {
        if(run.number==runNumber)
        {
            return run;
        }
    }

    cerr << "Error: failed to find run " << runNumber
        << " in config file. Returning empty run." << endl;
    return Run();
}

Angle Config::getAngle(double value)
{
    for(auto& angle : angles)
    {
        if(angle.value==value)
        {
            return angle;
        }
    }

    cerr << "Error: failed to find angle " << value
        << " in config file. Returning empty Angle." << endl;
    return Angle();
}
