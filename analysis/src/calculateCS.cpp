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

                // calculate counts per monitor in target histo (background
                // subtracted)
                double difference = (targetCounts/targetMonitors)-(blankCounts/blankMonitors);

                // convert from lab angle to CM angle
                double CMAngle = labAngleToCMAngle(angle.value, NEUTRON_MASS, t.getMolarMass());

                double targetNumberOfAtoms = (t.getMass()/t.getMolarMass())*AVOGADROS_NUMBER;

                // calculate differential cross section in lab frame
                double value = reference.crossSection*
                    (difference/reference.difference[i])*
                    (2*reference.polyNumberOfAtoms/targetNumberOfAtoms);

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
                            pow(reference.polyCounts[i]/reference.polyMonitors[i],2)
                           *(1/reference.polyCounts[i]+1/reference.polyMonitors[i])

                          + pow(reference.graphiteCounts[i]/reference.graphiteMonitors[i],2)
                           *pow(reference.polyNumberOfAtoms/reference.graphiteNumberOfAtoms,2)
                           *(1/reference.graphiteCounts[i]+1/reference.graphiteMonitors[i])

                          + pow(reference.blankCounts[i]/reference.blankMonitors[i],2)
                           *pow((reference.polyNumberOfAtoms/reference.graphiteNumberOfAtoms)-1,2)
                           *(1/reference.blankCounts[i]+1/reference.blankMonitors[i])

                          + pow(reference.polyNumberOfAtoms/reference.graphiteNumberOfAtoms,2)
                           *pow(reference.blankCounts[i]/reference.blankMonitors[i]-reference.graphiteCounts[i]/reference.graphiteMonitors[i],2)
                           *(1/reference.polyNumberOfAtoms+1/reference.graphiteNumberOfAtoms)
                           ,0.5
                       );

                //cout << "referenceStatisticalError = " << 100*referenceStatisticalError/reference.difference[i] << "%" << endl;
                    
                double statisticalError = value*
                    pow(
                            pow(referenceStatisticalError/reference.difference[i],2)
                          + pow(targetStatisticalError/difference,2)
                           ,0.5
                       );

                //cout << "Statistical error = " << statisticalError << endl;

                fileOut << CMAngle << " " << value
                    << " " << statisticalError << endl;

                // need to add efficiency correction for energy drop at high angles affecting count
                // rates
            }
        }
    }

    return 0;
}
