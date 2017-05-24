#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

struct ElasticCSDataPoint
{
    ElasticCSDataPoint(double d, double ecs, double e) : degrees(d), elasticCS(ecs), error(e) {}

    double degrees;
    double elasticCS;
    double error;
};

struct RawDataPoint
{
    double angle = 0;
    unsigned int Det4M = 0;
    unsigned int Det6M = 0;
    unsigned int Mon0Degree = 0;
    unsigned int MonCeiling = 0;
};

struct TargetData
{
    double numberOfAtoms = 0;
    double ControlCS = 0;
};

#endif /* DATA_STRUCTURES_H */
