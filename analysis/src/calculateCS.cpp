// For calculating elastic cross section data using raw experimental data and
// by scaling an existing literature data point

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <string>
#include <vector>
#include <iomanip>

#include "TFile.h"
#include "TH1I.h"

#include "../include/DataStructures.h"
#include "../include/Config.h"
#include "../include/experimentalConstants.h"
#include "../include/physicalConstants.h"
#include "../include/ReferenceCS.h"

extern Config config;

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

int calculateCS(const ReferenceCS& reference)
{
    vector<string> detectorNames = {"4M", "6M"};

    string referenceFileLocation = "../configuration/" + config.experiment
        + "/normalization/";

    for(auto& name : TARGET_NAMES)
    {
        Target t = Target("../configuration/" + config.experiment
            + "/targets/" + name + "/");

        for(int i=0; i<detectorNames.size(); i++)
        {
            ofstream fileOut("literatureData/" + name + "_" + detectorNames[i] + ".txt");
            fileOut << endl << name << ", " << detectorNames[i] << endl;
            fileOut << "Degrees    mB/sr      Error (mB/sr)" << endl;

            for(auto& angle : config.angles)
            {
                // read histogram counts for each run
                stringstream ss;
                ss << setprecision(5) << angle.value;
                string histoFileName = "../processedData/" + config.experiment + "/angles/" + ss.str() + "/" + name + ".root";
                TFile histoFile(histoFileName.c_str(),"UPDATE");

                if(!histoFile.IsOpen())
                {
                    continue;
                }

                string histoName = "diff" + detectorNames[i];
                TH1I* histo = (TH1I*)histoFile.Get(histoName.c_str());

                if(!histo)
                {
                    continue;
                }

                // extract counts from histo based on histogram gates
                int minIntBin = histo->FindBin(t.intLimits.low[i]);
                int maxIntBin = histo->FindBin(t.intLimits.high[i]);

                double difference = histo->Integral(minIntBin, maxIntBin);

                // convert from lab angle to CM angle
                double CMAngle = labAngleToCMAngle(angle.value, NEUTRON_MASS, t.getMass());

                double targetNumberOfAtoms = (t.getMass()/t.getMolarMass())*AVOGADROS_NUMBER;

                if((reference.counts.size()-1 < i)
                        || (reference.monitors.size()-1 < i))
                {
                    cerr << "Error: reference counts or monitors not available for detector " << detectorNames[i]
                        << endl;
                    return 1;
                }

                /*cout << "For " << detectorNames[i] << " at " << angle.value << " degrees, integrated diff = " << difference
                    << ", reference counts = " << reference.counts[i]
                    << endl;
                    */

                // calculate differential cross section in lab frame
                double value = ((double)difference/reference.counts[i])*
                    ((double)reference.monitors[i]/NORMALIZATION_SCALING)*
                    (reference.polyNumberOfAtoms/targetNumberOfAtoms)*
                    reference.crossSection;

                // convert lab frame cross section to center-of-mass frame via
                // Jacobian
                value *= labToCMJacobian(angle.value, NEUTRON_MASS, t.getMass());

                fileOut << angle.value << " " << value
                    << " " << "0" << endl;

                // need to add efficiency correction for energy drop at high angles affecting count
                // rates
            }
        }
    }

    return 0;
}
