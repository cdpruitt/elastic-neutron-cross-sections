#include "../include/utilities.h"
#include "../include/physicalConstants.h"

#include <iostream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    if(argc<5 || argc>5)
    {
        cerr << "Error: calculateTOF() requires 4 arguments, but " << argc-1
            << " were encountered. Format must be:" << endl;

        cerr << "   ./calculateTOF <neutronEnergy> <targetMass> <detectorAngle> <detectorDistance>" << endl;
        return 1;
    }

    double neutronEnergy = stod(argv[1]);
    double targetMass = stod(argv[2]);
    double detectorAngle = stod(argv[3]);
    double detectorDistance = stod(argv[4]);

    if(neutronEnergy < 0 || targetMass < 0
            || detectorAngle < 0 || detectorAngle > 180
            || detectorDistance < 0)
    {
        cerr << "Error: one or more entered parameters is non-physical. Exiting..."
            << endl;
        return 1;
    }

    double neutronEnergyAfter = calculateScatteredEnergy(
            neutronEnergy,
            0,
            NEUTRON_MASS*AMU_TO_MEVC2,
            targetMass*AMU_TO_MEVC2,
            detectorAngle);

    double TOF = calculateTOF(detectorDistance, 0, targetMass, detectorAngle, neutronEnergy);

    cout << "For " << neutronEnergy << " MeV neutron elastically scattering on "
        << targetMass << " amu target to a lab angle of " << detectorAngle
        << " degrees:" << endl;
    
    cout << "Center-of-mass angle = " << labAngleToCMAngle(detectorAngle, NEUTRON_MASS, targetMass) << endl;
    cout << "Neutron energy after collision = " << neutronEnergyAfter << endl;
    cout << "TOF to detector at " << detectorDistance << " cm = " << TOF << endl;

    return 0;
}
