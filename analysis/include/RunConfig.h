#ifndef RUN_CONFIG_H
#define RUN_CONFIG_H

#include <vector>
#include <string>
#include <iostream>

#include "TH1.h"

#include "Target.h"
#include "experimentalConstants.h"

struct RunConfig
{
    RunConfig() {}
    RunConfig(std::vector<std::string> configData) :
        runNumber(stoi(configData[0])), targetName(configData[1]),
        angle4M(stod(configData[2])), angle6M(stod(configData[3]))
    {}

    int runNumber;
    std::string targetName;
    double angle4M;
    double angle6M;
};

struct AllConfigs
{
    std::vector<RunConfig> runs;

    RunConfig getRunConfig(int runNumber);
};

struct AngleData
{
    AngleData()
    {
        for(auto& name : TARGET_NAMES)
        {
            targets.push_back(Target(name));
        }
    };

    double angle;

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

        std::cerr << "Error: couldn't find blank target in angle " << std::to_string(angle)
            << ". Returning empty target..." << std::endl;
        return Target();
    };

    bool operator<(const AngleData& comparand) const
    {
        return (angle < comparand.angle);
    }
};

struct AllAngles
{
    std::vector<AngleData> angles;
    
    AngleData getAngle(double angle);
    void getRunData(AllConfigs runs);
};

AllConfigs getRunConfig();

#endif /* RUN_CONFIG_H */
