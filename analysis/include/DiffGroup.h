#ifndef DIFF_GROUP_H
#define DIFF_GROUP_H

#include <vector>

struct DiffGroup
{
    int groupNumber;
    double angle4M;
    double angle6M;
    int blank;
    int target1;
    int target2;
};

std::vector<DiffGroup> readDiffHistoConfig();
DiffGroup getDiffGroup(int groupNumber);

#endif /* DIFF_GROUP_H */
