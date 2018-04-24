#ifndef DIFF_GROUP_H
#define DIFF_GROUP_H

#include <vector>

struct DiffGroup
{
    int groupNumber;
    double angle;
    int blank;
    int target1;
    int target2;
};

std::vector<DiffGroup> readDiffHistoConfig();

#endif /* DIFF_GROUP_H */
