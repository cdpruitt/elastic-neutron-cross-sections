// defines the format of scaler data for each run of this experiment.
// More context is available in the data/experimentalConfig.txt file.

#ifndef SCALER_DATA_H
#define SCALER_DATA_H

#include <vector>
#include <string>

struct ScalerData
{
    ScalerData(std::string scalerFileName);

    unsigned long long int tenHzClock;
    unsigned long long int BP0; // prescale = 10
    unsigned long long int BCI; // 10^-8 coulombs/pulse
    unsigned long long int Det4mCFD;
    unsigned long long int Det6mCFD;
    unsigned long long int Det0DegreeCFD; // raw 0-degree detector CFD
    unsigned long long int Det0DegreeCFD_100; // 0-degree detector CFD, divided by 100
    unsigned long long int DetCMonCFD; // raw ceiling monitor detector CFD
    unsigned long long int DetCMonCFD_10; // ceiling monitor detector CFD, divided by 10
    unsigned long long int VetoClock;
    unsigned long long int sixtyHzClock;
    unsigned long long int triggers;
    unsigned long long int liveTriggers;
};

#endif /* SCALER_DATA_H */
