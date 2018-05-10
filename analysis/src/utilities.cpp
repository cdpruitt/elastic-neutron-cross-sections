#include "../include/utilities.h"
#include <cmath>

#include "../include/physicalConstants.h"

using namespace std;

// convert a lab angle (in degrees) to a center-of-mass angle (in degrees),
// ranging from 0-180
// (see "Classical Dynamics of Particles and Systems", 3rd Edition, by Marion
// and Thornton, page 310)
double labAngleToCMAngle(double labAngle, double massOfProjectile, double massOfTarget)
{
    double tanCMAngle = sin(labAngle*(M_PI/180))/(cos(labAngle*M_PI/180)-massOfProjectile/massOfTarget);
    double cmAngle = (180/M_PI)*atan2(tanCMAngle, 1);

    // translate output range from (-90,90) to (0,180)
    if(cmAngle<0)
    {
        cmAngle += 180;
    }

    return cmAngle;
}

// convert a center-of-mass angle (in degrees) to a lab angle (in degrees),
// ranging from 0-180
// (see "Classical Dynamics of Particles and Systems", 3rd Edition, by Marion
// and Thornton, page 310)
double CMAngleToLabAngle(double CMAngle, double massOfProjectile, double massOfTarget)
{
    double tanLabAngle = sin(CMAngle*(M_PI/180))/(cos(CMAngle*M_PI/180)+massOfProjectile/massOfTarget);
    double labAngle = (180/M_PI)*atan2(tanLabAngle, 1);

    // translate output range from (-90,90) to (0,180)
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
        double projectileMass, // in amu
        double targetMass, // in amu
        double labAngle // in degrees
        )
{
    // calculate lab frame velocity of neutron before collision
    double neutronVelBefore = pow(2*projectileEnergy/projectileMass,0.5);

    // calculate velocity of the center of mass frame with respect to the lab
    // frame
    double CMvelocity = neutronVelBefore*(projectileMass/(projectileMass+targetMass));

    double CMangle = labAngleToCMAngle(labAngle, projectileMass, targetMass);

    // calculate velocity of projectile in center of mass frame
    double neutronVCM = neutronVelBefore*(1-projectileMass/(projectileMass+targetMass));

    // calculate velocity of projectile in lab frame after collision
    double neutronVlabX = CMvelocity + neutronVCM*cos(CMangle*(M_PI/180));
    double neutronVlabY = neutronVCM*sin(CMangle*(M_PI/180));
    double neutronVlabAfter = pow(pow(neutronVlabX,2) + pow(neutronVlabY,2),0.5);

    // calculate energy of projectile in lab frame after collision
    return 0.5*projectileMass*pow(neutronVlabAfter,2);
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
        Detector d,
        Target t,
        double angle,
        double neutronEnergyBefore
        )
{
    // calculate neutron energy after scattering to lab angle
    double neutronEnergyAfter = calculateScatteredEnergy(
            neutronEnergyBefore,
            NEUTRON_MASS,
            t.getMolarMass(),
            angle);

    double neutronTOFAngle = d.distance/pow(2*neutronEnergyAfter/NEUTRON_MASS, 0.5);

    return neutronTOFAngle;
}
