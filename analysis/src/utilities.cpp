#include "../include/utilities.h"
#include <cmath>

#include "../include/physicalConstants.h"
#include <iostream>

using namespace std;

// convert a lab angle (in degrees) to a center-of-mass angle (in degrees),
// ranging from 0-180
// (see "Classical Dynamics of Particles and Systems", 3rd Edition, by Marion
// and Thornton, page 310)
double labAngleToCMAngle(double labAngle, double massOfProjectile, double massOfTarget)
{
    /*double tanCMAngle = sin(labAngle*(M_PI/180))/(cos(labAngle*M_PI/180))*(massOfProjectile+massOfTarget/massOfTarget);
    cout << "tanCMAngle = " << tanCMAngle << endl;
    double cmAngle = (180/M_PI)*atan2(tanCMAngle, 1);

    // convert from (-90,90) to (0,180)
    if(cmAngle<0)
    {
        cmAngle += 180;
    }

    return cmAngle;
    */

    double massRatio = massOfProjectile/massOfTarget;
    double lA = labAngle*M_PI/180;

    double cosCMAngle = cos(lA)*pow(1-pow(massRatio*sin(lA),2),0.5)-massRatio*pow(sin(lA),2);
    return (180/M_PI)*acos(cosCMAngle);
}

// convert a center-of-mass angle (in degrees) to a lab angle (in degrees),
// ranging from 0-180
double CMAngleToLabAngle(double CMAngle, double massOfProjectile, double massOfTarget)
{
    double tanLabAngle = sin(CMAngle*(M_PI/180))/(cos(CMAngle*M_PI/180)+massOfProjectile/massOfTarget);
    double labAngle = (180/M_PI)*atan2(tanLabAngle,1);

    // convert from (-90,90) to (0,180)
    if(labAngle<0)
    {
        labAngle += 180;
    }

    return labAngle;
}

// calculate the Jacobian for the transformation from the lab frame to the center-of-mass frame
// (see "Classical Dynamics of Particles and Systems", 3rd Edition, by Marion
// and Thornton, page 326)
double labToCMJacobian(double labAngle, double massOfProjectile, double massOfTarget)
{
    double labRadians = labAngle*(M_PI/180);
    double massRatio = massOfProjectile/massOfTarget;

    double numerator = sqrt(1-pow(massRatio*sin(labRadians),2));
    double denominator = pow(massRatio*cos(labRadians) + numerator,2);

    return numerator/denominator;
}

double calculateScatteredEnergy(
        double projectileEnergy, // projectile energy before collision, in MeV
        double excitationEnergy, // energy removed from elastic scattering
        double projectileMass, // in MeV/c^2
        double targetMass, // in MeV/c^2
        double labAngle // in degrees
        )
{
    if(targetMass==0)
    {
        return projectileEnergy-excitationEnergy;
    }

    // calculate lab frame velocity of neutron before collision (minus any
    // future inelastic excitation)
    double neutronVelBefore = pow(2*(projectileEnergy)/projectileMass,0.5); // as fraction of C

    // calculate velocity of the center of mass frame with respect to the lab
    // frame
    double CMVelocity = neutronVelBefore*(projectileMass/(projectileMass+targetMass));
    double CMAngle = labAngleToCMAngle(labAngle, projectileMass, targetMass);

    // calculate velocity of projectile in center of mass frame before
    // scattering
    double neutronVCM = neutronVelBefore-CMVelocity;
    double targetVCM = -neutronVCM*(projectileMass/targetMass);

    // calculate fraction of velocity lost by inelastic excitation during
    // scattering
    double neutronVCMAfter = pow(
            (pow(neutronVCM,2)*(projectileMass+pow(projectileMass,2)/targetMass)-2*excitationEnergy)
            /(projectileMass+pow(projectileMass,2)/targetMass)
            ,0.5);
    double targetVCMAfter = -neutronVCMAfter*(projectileMass/targetMass);

    // calculate velocity of projectile in center of mass frame after scattering
    double neutronVCMX = neutronVCMAfter*cos(CMAngle*(M_PI/180));
    double neutronVCMY = neutronVCMAfter*sin(CMAngle*(M_PI/180));

    // calculate velocity of target in lab frame after collision
    double targetVCMX = targetVCMAfter*cos(CMAngle*(M_PI/180));
    double targetVCMY = targetVCMAfter*sin(CMAngle*(M_PI/180));

    // calculate energy of projectile in lab frame after collision
    double projectileEnergyAfter = 0.5*projectileMass*(pow(neutronVCMX+CMVelocity,2)+pow(neutronVCMY,2)); // in MeV
    double targetEnergyAfter = 0.5*targetMass*(pow(targetVCMX+CMVelocity,2)+pow(targetVCMY,2)); // in MeV

    return projectileEnergyAfter;
}

double calculateTOF(
        double distance,
        double projectileEnergy,
        double projectileMass
        )
{
    return distance/(pow(2*projectileEnergy/projectileMass, 0.5));
}

double calculateTOF(
        double distance,
        double excitationEnergy,
        double targetMass,
        double angle,
        double neutronEnergyBefore
        )
{
    // calculate neutron energy after scattering to lab angle
    double neutronEnergyAfter = calculateScatteredEnergy(
            neutronEnergyBefore,
            excitationEnergy,
            NEUTRON_MASS*AMU_TO_MEVC2,
            targetMass*AMU_TO_MEVC2,
            angle);

    return distance/pow(2*neutronEnergyAfter/NEUTRON_MASS, 0.5);
}
