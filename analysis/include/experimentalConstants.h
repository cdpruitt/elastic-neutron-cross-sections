#ifndef EXPERIMENTAL_CONSTANTS_H
#define EXPERIMENTAL_CONSTANTS_H

#include <vector>

const unsigned int SIX_METER_PULSE_HEIGHT_THRESHOLD = 350;
const unsigned int FOUR_METER_PULSE_HEIGHT_THRESHOLD = 350;

const unsigned int SIX_METER_PSD_THRESHOLD = 1300;
const unsigned int FOUR_METER_PSD_THRESHOLD = 1500;

const unsigned int SIX_METER_TIME_MINIMUM = 57;
const unsigned int SIX_METER_TIME_MAXIMUM = 84;

const unsigned int FOUR_METER_TIME_MINIMUM = 73;
const unsigned int FOUR_METER_TIME_MAXIMUM = 94;

const std::vector<std::string> DETECTOR_NAMES =
{"4M", "6M", "0DEG", "CMON", "RF"};

#endif /* EXPERIMENTAL_CONSTANTS_H */
