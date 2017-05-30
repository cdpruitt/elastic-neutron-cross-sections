// defines the format of scaler data for each run of this experiment.
// More context is available in the data/experimentalConfig.txt file.

#ifndef SCALER_DATA_H
#define SCALER_DATA_H

#include <vector>
#include <string>

struct ScalerData
{
    ScalerData(std::string scalerFileName);

    unsigned int tenHzClock;
    unsigned int BP0; // prescale = 10
    unsigned int BCI; // 10^-8 coulombs/pulse
    unsigned int Det4mCFD;
    unsigned int Det6mCFD;
    unsigned int Det0DegreeCFD; // raw 0-degree detector CFD
    unsigned int Det0DegreeCFD_100; // 0-degree detector CFD, divided by 100
    unsigned int DetCMonCFD; // raw ceiling monitor detector CFD
    unsigned int DetCMonCFD_10; // ceiling monitor detector CFD, divided by 10
    unsigned int VetoClock;
    unsigned int sixtyHzClock;
    unsigned int triggers;
    unsigned int liveTriggers;
}

#endif /* SCALER_DATA_H */
