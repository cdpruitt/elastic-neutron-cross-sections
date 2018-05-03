#ifndef EXPERIMENTAL_CONSTANTS_H
#define EXPERIMENTAL_CONSTANTS_H

#include <vector>

const int SIX_METER_PULSE_HEIGHT_THRESHOLD = 350;
const int FOUR_METER_PULSE_HEIGHT_THRESHOLD = 350;

const int SIX_METER_PSD_THRESHOLD = 1300;
const int FOUR_METER_PSD_THRESHOLD = 1500;

const int SIX_METER_TIME_MINIMUM = 57;
const int SIX_METER_TIME_MAXIMUM = 84;

const int FOUR_METER_TIME_MINIMUM = 73;
const int FOUR_METER_TIME_MAXIMUM = 94;

const double NORMALIZATION_SCALING = 300000;

const std::vector<std::string> TARGET_NAMES =
{"Sn112", "Sn124", "blank", "SnNat"};

const std::vector<std::string> DETECTOR_NAMES =
{"4M", "6M", "0DEG", "CMON", "RF"};

#endif /* EXPERIMENTAL_CONSTANTS_H */
