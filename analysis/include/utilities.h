#ifndef UTILITIES_H
#define UTILITIES_H

#include "Target.h"
#include "Detector.h"

double labAngleToCMAngle(double labAngle, double massOfProjectile, double massOfTarget);
double CMAngleToLabAngle(double CMAngle, double massOfProjectile, double massOfTarget);

double labToCMJacobian(double labAngle, double massOfProjectile, double massOfTarget);

double calculateScatteredEnergy(
        double projectileEnergy, // incident neutron energy, in MeV
        double excitationEnergy, // energy of inelastic excitation, if any, in MeV
        double projectileMass,
        double targetMass, // in amu
        double labAngle // in degrees
        );

double calculateTOF(
        double distance,
        double excitationEnergy,
        double targetMass,
        double angle,
        double neutronEnergyBefore
        );

#endif /* UTILITIES_H */
