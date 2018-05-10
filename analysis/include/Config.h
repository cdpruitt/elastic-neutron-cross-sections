#ifndef RUN_CONFIG_H
#define RUN_CONFIG_H

#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "Target.h"
#include "Detector.h"

struct Run
{
    Run() {}
    Run(std::vector<std::string> configData) :
        number(stoi(configData[0])), target(configData[1])
    {
        angles.push_back(stod(configData[2]));
        angles.push_back(stod(configData[3]));
    }

    int number;
    std::string target;
    std::vector<double> angles;
};

struct Angle
{
    double value;

    std::vector<Run> runs;
    std::vector<Target> targets;

    Target getBlank()
    {
        for(auto& target : targets)
        {
            if(target.name=="blank")
            {
                return target;
            }
        }

        std::cerr << "Error: couldn't find blank target in angle " << std::setprecision(5) << std::to_string(value)
            << ". Returning empty target..." << std::endl;
        return Target();
    };

    bool operator<(const Angle& comparand) const
    {
        return (value < comparand.value);
    }
};

struct Config
{
    Config() {};
    Config(std::string fileName);

    std::string experiment;
    std::vector<Run> runs;
    std::vector<Angle> angles;
    std::vector<Detector> detectors;

    Run getRun(int runNumber);
    Angle getAngle(double angle);

    double neutronEnergy;
};

#endif /* RUN_CONFIG_H */
