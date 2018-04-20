#ifndef RUN_CONFIG_H
#define RUN_CONFIG_H

#include <vector>
#include <string>

struct RunConfig
{
    RunConfig() {}
    RunConfig(std::vector<std::string> configData) :
        runNumber(stoi(configData[0])), targetName(configData[1]),
        angle(stod(configData[2])) {}

    int runNumber;
    std::string targetName;
    double angle;
};

std::vector<RunConfig> getRunConfig(std::string detectorName);

#endif /* RUN_CONFIG_H */
