#ifndef TARGET_H
#define TARGET_H

#include <string>
#include <utility>
#include "TH1.h"

#include "IntegrationLimits.h"

struct Target
{
    Target() {};
    Target(std::string targetDataLocation);

    std::string name;

    std::vector<std::vector<TH1D*>> histos;
    std::vector<std::vector<TH1D*>> monitors;

    IntegrationLimits intLimits;

    std::string getName() const;
    double getLength() const;
    double getDiameter() const;
    double getMass() const;
    double getMolarMass() const;

    double getDiameterUncertainty() const;
    double getMassUncertainty() const;
    double getMolarMassUncertainty() const;

    void setName(std::string n);
    void setLength(double l);
    void setDiameter(double d);
    void setMass(double m);
    void setMolarMass(double mm);

    void setDiameterUncertainty(double d);
    void setMassUncertainty(double m);
    void setMolarMassUncertainty(double mm);

    private:
    // physical target parameters
    double length;
    double diameter;
    double mass;
    double molMass;

    double diameterUncertainty;
    double massUncertainty;
    double molMassUncertainty;
};

#endif /* TARGET_H */
