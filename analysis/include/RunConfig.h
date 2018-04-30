#ifndef RUN_CONFIG_H
#define RUN_CONFIG_H

#include <vector>
#include <string>

#include "TH1.h"

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
    double angle;
    std::vector<TH1D*> Sn112Histo4M;
    std::vector<TH1D*> Sn112Monitor4M;
    std::vector<TH1D*> Sn112Histo6M;
    std::vector<TH1D*> Sn112Monitor6M;

    std::vector<TH1D*> Sn124Histo4M;
    std::vector<TH1D*> Sn124Monitor4M;
    std::vector<TH1D*> Sn124Histo6M;
    std::vector<TH1D*> Sn124Monitor6M;

    std::vector<TH1D*> blankHisto4M;
    std::vector<TH1D*> blankMonitor4M;
    std::vector<TH1D*> blankHisto6M;
    std::vector<TH1D*> blankMonitor6M;

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
