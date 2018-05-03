#ifndef PID_BANANA_H
#define PID_BANANA_H

#include <string>
#include <vector>

struct PIDBanana
{
    PIDBanana() {};
    PIDBanana(std::string detectorName);

    std::vector<float> X;
    std::vector<float> Y;
};

#endif /* PID_BANANA_H */
