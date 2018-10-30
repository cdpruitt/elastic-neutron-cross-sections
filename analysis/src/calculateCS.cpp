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
#include "TH1.h"

#include "../include/DataStructures.h"
#include "../include/Config.h"
#include "../include/experimentalConstants.h"
#include "../include/physicalConstants.h"
#include "../include/ReferenceCS.h"
#include "../include/utilities.h"

extern Config config;

using namespace std;

int calculateCS(const vector<ReferenceCS>& references)
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
                continue;
            }

            ofstream fileOut("literatureData/" + name + "_" + d.name + "_" + config.experiment + ".txt");
            fileOut << endl << name << ", " << d.name << ", " << config.experiment << endl;
            fileOut << "Degrees    mB/sr      Error (mB/sr)" << endl;

            for(auto& angle : config.angles)
            {
                // read histogram counts for each run
                stringstream ss;
                ss << setprecision(5) << angle.value;
                string histoFileName = "../processedData/" + config.experiment + "/angles/" + ss.str() + "/" + name + ".root";

                ifstream file(histoFileName.c_str());
                if(!file.is_open())
                {
                    continue;
                }
                file.close();

                TFile histoFile(histoFileName.c_str(),"READ");

                if(!histoFile.IsOpen())
                {
                    continue;
                }

                string histoName = "histo" + d.name + "Total";
                TH1D* histo = (TH1D*)histoFile.Get(histoName.c_str());

                if(!histo)
                {
                    continue;
                }

                string blankName = "blank" + d.name + "Total";
                TH1D* blank = (TH1D*)histoFile.Get(blankName.c_str());

                if(!blank)
                {
                    continue;
                }

                string monitorName = "monitor" + d.name + "Total";
                TH1D* monitor = (TH1D*)histoFile.Get(monitorName.c_str());

                if(!monitor)
                {
                    continue;
                }

                string blankMonitorName = "blankMonitor" + d.name + "Total";
                TH1D* blankMonitor = (TH1D*)histoFile.Get(blankMonitorName.c_str());

                if(!blankMonitor)
                {
                    continue;
                }

                double neutronTOF = calculateTOF(d.distance, 0, t.getMolarMass(), angle.value, config.neutronEnergy);

                double TOFResolution = d.resolution*d.linearCalibration; // FWHM in ns

                double targetCounts = histo->Integral(
                        histo->FindBin(neutronTOF-TOFResolution),
                        histo->FindBin(neutronTOF+TOFResolution)
                        );

                double blankCounts = blank->Integral(
                        blank->FindBin(neutronTOF-TOFResolution),
                        blank->FindBin(neutronTOF+TOFResolution)
                        );

                double targetMonitors = monitor->GetEntries();
                double blankMonitors = blankMonitor->GetEntries();

                /*cout << "For angle = " << angle.value << " " << d.name << " " << name;
                cout << ", target counts = " << targetCounts << ", blank counts = " << blankCounts;
                cout << ", target mons = " << targetMonitors << ", blank mons = " << blankMonitors << endl;
                */

                // calculate counts per monitor in target histo (background
                // subtracted)
                double difference = (targetCounts/targetMonitors)-(blankCounts/blankMonitors);

                double targetNumberOfAtoms = (t.getMass()/t.getMolarMass())*AVOGADROS_NUMBER;

                // calculate differential cross section in lab frame
                double value = references[i].crossSection*
                    (difference/references[i].difference)*
                    (2*references[i].polyNumberOfAtoms/targetNumberOfAtoms);
                // ^^ factor of 2 from stoichiometry of H in CH2

                // convert lab frame cross section to center-of-mass frame via
                // Jacobian
                value *= labToCMJacobian(angle.value, NEUTRON_MASS, t.getMass());

                // calculate cross section error
                double targetStatisticalError =
                    pow(
                            pow(targetCounts/targetMonitors,2)
                           *(1/targetCounts+1/targetMonitors)
                          + pow(blankCounts/blankMonitors,2)
                           *(1/blankCounts+1/blankMonitors)
                           ,0.5
                       );

                //cout << "targetStatisticalError = " << 100*targetStatisticalError/difference << "%" << endl;

                double referenceStatisticalError =
                    pow(
                            pow(references[i].polyCounts/references[i].polyMonitors,2)
                           *(1/references[i].polyCounts+1/references[i].polyMonitors)

                          + pow(references[i].graphiteCounts/references[i].graphiteMonitors,2)
                           *pow(references[i].polyNumberOfAtoms/references[i].graphiteNumberOfAtoms,2)
                           *(1/references[i].graphiteCounts+1/references[i].graphiteMonitors)

                          + pow(references[i].blankCounts/references[i].blankMonitors,2)
                           *pow((references[i].polyNumberOfAtoms/references[i].graphiteNumberOfAtoms)-1,2)
                           *(1/references[i].blankCounts+1/references[i].blankMonitors)

                          + pow(references[i].polyNumberOfAtoms/references[i].graphiteNumberOfAtoms,2)
                           *pow(references[i].blankCounts/references[i].blankMonitors-references[i].graphiteCounts/references[i].graphiteMonitors,2)
                           *(1/references[i].polyNumberOfAtoms+1/references[i].graphiteNumberOfAtoms)
                           ,0.5
                       );

                //cout << "referenceStatisticalError = " << 100*referenceStatisticalError/reference.difference[i] << "%" << endl;
                    
                double statisticalError = value*
                    pow(
                            pow(referenceStatisticalError/references[i].difference,2)
                          + pow(targetStatisticalError/difference,2)
                           ,0.5
                       );

                //cout << "Statistical error = " << statisticalError << endl;

                // convert from lab angle to CM angle
                double CMAngle = labAngleToCMAngle(angle.value, NEUTRON_MASS, t.getMolarMass());

                fileOut << CMAngle << " " << value
                    << " " << statisticalError << endl;

                // need to add efficiency correction for energy drop at high angles affecting count
                // rates
            }
        }
    }

    return 0;
}
