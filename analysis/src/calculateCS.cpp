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
#include "../include/utilities.h"

extern Config config;

using namespace std;

int calculateCS(const ReferenceCS& reference)
{
    for(auto& name : TARGET_NAMES)
    {
        Target t = Target("../configuration/" + config.experiment
            + "/targets/" + name + "/");

        for(int i=0; i<config.detectors.size(); i++)
        {
            auto& d = config.detectors[i];
            if(!d.useForCS)
            {
                cout << "Skipping detector " << d.name << " for cross section calculation." << endl;
                continue;
            }

            ofstream fileOut("literatureData/" + name + "_" + d.name + ".txt");
            fileOut << endl << name << ", " << d.name << endl;
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

                string histoName = "diff" + d.name;
                TH1I* histo = (TH1I*)histoFile.Get(histoName.c_str());

                if(!histo)
                {
                    continue;
                }

                double neutronTOF = calculateTOF(d.distance, 0, t.getMolarMass(), angle.value, config.neutronEnergy);

                double TOFResolution = d.resolution*d.linearCalibration; // FWHM in ns
                double difference = histo->Integral(
                        histo->FindBin(neutronTOF-TOFResolution),
                        histo->FindBin(neutronTOF+TOFResolution)
                        );

                // convert from lab angle to CM angle
                double CMAngle = labAngleToCMAngle(angle.value, NEUTRON_MASS, t.getMolarMass());

                double targetNumberOfAtoms = (t.getMass()/t.getMolarMass())*AVOGADROS_NUMBER;

                if((reference.counts.size()-1 < i)
                        || (reference.monitors.size()-1 < i))
                {
                    cerr << "Error: reference counts or monitors not available for detector " << d.name
                        << endl;
                    return 1;
                }

                // calculate differential cross section in lab frame
                double value = ((double)difference/reference.counts[i])*
                    ((double)reference.monitors[i]/NORMALIZATION_SCALING)*
                    (2*reference.polyNumberOfAtoms/targetNumberOfAtoms)*
                    reference.crossSection;

                cout << "For detector " << d.name << ", reference counts = " << reference.counts[i] << endl;

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
