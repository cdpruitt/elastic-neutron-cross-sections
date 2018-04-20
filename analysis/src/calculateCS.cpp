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
#include "../include/ScalerData.h"
#include "../include/RunConfig.h"

using namespace std;

const double REFERENCE_CS = 229.4;
const double REFERENCE_NUMBER_OF_ATOMS = 2.91e23;
const double Sn112_NUMBER_OF_ATOMS = 2.676e22;
const double Sn124_NUMBER_OF_ATOMS = 2.697e22;
double TARGET_NUMBER_OF_ATOMS;
const double REFERENCE_4M_COUNTS = 6596; // integral gates are 1194->1260
const double REFERENCE_6M_COUNTS = 4258; // integral gates are 780->885
double REFERENCE_HISTO_COUNTS = 0;
const double TIME_CALIBRATION_FACTOR = 5.668 // ch/ns

const unsigned int BCI_REFERENCE_VALUE = 500000;

void readTargetDataFile(TargetData& targetData, string fileName)
{
    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cerr << "Error: failed to open " << fileName << ". Exiting..." << endl;
        exit(1);
    }

    //ignore two label lines
    string dummy, dummy2;
    getline(file, dummy);

    file >> dummy;
    targetData.numberOfAtoms = stod(dummy);

    file >> dummy2;
    if(dummy2!="")
    {
        targetData.ControlCS = stod(dummy2);
    }
}

struct CSPrereqs
{
    CSPrereqs() {}
    RunConfig runConfig;
    unsigned int BCICounts;
    unsigned int histoCounts;
};

struct CrossSection
{
    string target;
    double angle;
    double value;
    double error;
};

vector<unsigned int> getIntegralBounds(string integralBoundsFileName)
{
    ifstream file(integralBoundsFileName.c_str());
    if(!file.is_open())
    {
        cerr << "Failed to find integral bounds data in " << integralBoundsFileName << std::endl;
        exit(1);
    }

    vector<unsigned int> integralBounds;

    string dummy;
    getline(file,dummy);

    unsigned int lowBound;
    unsigned int highBound;

    file >> lowBound >> highBound;

    integralBounds.push_back(lowBound);
    integralBounds.push_back(highBound);

    return integralBounds;
}

int getIntegralBoundsCorrection(string fileName)
{
    ifstream file(fileName.c_str());
    if(!file.is_open())
    {
        cerr << "Failed to find integral bounds data in " << fileName << std::endl;
        exit(1);
    }

    int integralBoundsCorrection;

    string dummy;
    getline(file,dummy);

    file >> integralBoundsCorrection;

    return (int)TIME_CALIBRATION_FACTOR*integralBoundsCorrection;
}

void outputCrossSections(const vector<CrossSection>& crossSections,
        string targetName, string detectorName)
{
    // read header material we should print at the start of output files
    string headerFileName = "configuration/output/" + targetName + "/" + detectorName + ".txt";
    ifstream headerFile(headerFileName);

    string dummy;
    getline(headerFile, dummy);
    getline(headerFile, dummy);

    vector<string> headerMaterial;

    while(getline(headerFile,dummy))
    {
        headerMaterial.push_back(dummy);
    }

    string outputFileName = "experimentalData/" + targetName + "_11_" + detectorName + ".txt";
    ofstream outputFile(outputFileName);

    for(string headerLine : headerMaterial)
    {
        outputFile << headerLine << endl;
    }

    for(CrossSection cs : crossSections)
    {
        if(cs.target == targetName)
        {
            outputFile << setw(11) << left << cs.angle << " " <<
                setw(11) << cs.value << " " <<
                setw(11) << cs.error << endl;
        }
    }
}

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

int main(int, char** argv)
{
    string detectorName = argv[1];

    if(detectorName=="4M")
    {
        REFERENCE_HISTO_COUNTS = REFERENCE_4M_COUNTS;
    }

    if(detectorName=="6M")
    {
        REFERENCE_HISTO_COUNTS = REFERENCE_6M_COUNTS;
    }

    vector<RunConfig> allConfigs = getRunConfig(detectorName);
    vector<CSPrereqs> allCSPrereqs;

    CSPrereqs reference; // for reference n,p target

    string integralBoundsFileName =
            "configuration/gates/" + detectorName + ".txt";

        vector<unsigned int> integralBounds =
            getIntegralBounds(integralBoundsFileName);

    for(RunConfig rc : allConfigs)
    {
        CSPrereqs csPrereqs;
        csPrereqs.runConfig = rc;

        // read beam flux information for this run
        string scalerFileName = "../rawData/runs/" + to_string(rc.runNumber) + "/scalers.txt";
        ScalerData sd(scalerFileName);
        csPrereqs.BCICounts = sd.BCI;

        // read histogram counts for each run
        string histoFileName = "../analyzedData/runs/" + to_string(rc.runNumber) + "/histos.root";
        TFile histoFile(histoFileName.c_str(),"UPDATE");

        string histoName = detectorName + "TDC";
        TH1I* histo = (TH1I*)histoFile.Get(histoName.c_str());

        string integralBoundsCorrectionFileName =
            "configuration/gates/" + rc.targetName + "/" + detectorName + "/" + runConfig.angle + ".txt";

        int integralBoundsCorrection =
            getIntegralBoundsCorrection(integralBoundsCorrectionFileName);

        // extract counts from histo based on histogram gates
        unsigned int lowBin = histo->GetBinLowEdge(0);
        csPrereqs.histoCounts = histo->Integral(integralBounds[0]-integralBoundsCorrection-lowBin,integralBounds[1]-integralBoundsCorrection-lowBin);

        allCSPrereqs.push_back(csPrereqs);

        // produce BCI-scaled histograms for this run
        string scaledHistoName = histoName + "Scaled";
        TH1I* scaledHisto = (TH1I*) histo->Clone(scaledHistoName.c_str());
        scaledHisto->Scale((double)BCI_REFERENCE_VALUE/sd.BCI);
        scaledHisto->Write();
    }

    // locate reference run
    for(int i=0; i<allCSPrereqs.size(); i++)
    {
        if(allCSPrereqs[i].runConfig.targetName=="polyethylene")
        {
            reference = allCSPrereqs[i];
            break;
        }
    }

    // combine runs with same run configurations
    vector<CSPrereqs> combinedCSPrereqs;
    for(int i=0; i<allCSPrereqs.size(); i++)
    {
        CSPrereqs currentCSPrereq = allCSPrereqs[i];
        for(int j=i+1; j<allCSPrereqs.size(); j++)
        {
            CSPrereqs trialCSPrereq = allCSPrereqs[j];
            if(
                    (trialCSPrereq.runConfig.targetName ==
                     currentCSPrereq.runConfig.targetName) &&
                    (trialCSPrereq.runConfig.angle ==
                     currentCSPrereq.runConfig.angle))
            {
                currentCSPrereq.BCICounts += trialCSPrereq.BCICounts;
                currentCSPrereq.histoCounts += trialCSPrereq.histoCounts;
            }
        }

        // don't add the same run config data twice
        bool unique = true;
        for(int j=0; j<combinedCSPrereqs.size(); j++)
        {
            if(
                    (currentCSPrereq.runConfig.targetName ==
                     combinedCSPrereqs[j].runConfig.targetName) &&
                    (currentCSPrereq.runConfig.angle ==
                     combinedCSPrereqs[j].runConfig.angle))
            {
                unique = false;
            }
        }

        if(unique)
        {
            combinedCSPrereqs.push_back(currentCSPrereq);
        }
    }

    // for each angle, find target and blank and perform the cross section
    // calculation
    vector<CrossSection> crossSections;

    double TARGET_MASS;
    const double NEUTRON_MASS = 1.008665; // in unified atomic mass units

    for(int i=0; i<combinedCSPrereqs.size(); i++)
    {
        CrossSection cs;

        // start by looking at runs with targets
        if(combinedCSPrereqs[i].runConfig.targetName=="blank")
        {
            continue;
        }

        if(combinedCSPrereqs[i].runConfig.targetName=="Sn112")
        {
           TARGET_NUMBER_OF_ATOMS = Sn112_NUMBER_OF_ATOMS; 
           cs.target = "Sn112";
           TARGET_MASS = 111.905; // in unified atomic mass units
        }

        else if(combinedCSPrereqs[i].runConfig.targetName=="Sn124")
        {
            TARGET_NUMBER_OF_ATOMS = Sn124_NUMBER_OF_ATOMS; 
            cs.target = "Sn124";
            TARGET_MASS = 123.905; // in unified atomic mass units
        }

        // convert from lab angle to CM angle
        double labAngle = combinedCSPrereqs[i].runConfig.angle;
        cs.angle = labAngleToCMAngle(labAngle, NEUTRON_MASS, TARGET_MASS);
        
        for(int j=0; j<combinedCSPrereqs.size(); j++)
        {
            if(
                    (combinedCSPrereqs[j].runConfig.angle==
                     combinedCSPrereqs[i].runConfig.angle) &&
                    (combinedCSPrereqs[j].runConfig.targetName == "blank"))
            {
                // calculate CS
                if(
                        (combinedCSPrereqs[i].histoCounts<=0) ||
                        (combinedCSPrereqs[j].histoCounts<=0) ||
                        (combinedCSPrereqs[i].BCICounts<=0) ||
                        (combinedCSPrereqs[j].BCICounts<=0))
                {
                    cerr << "Error: tried to calculate CS for angle = " <<
                        combinedCSPrereqs[i].runConfig.angle << ", but " <<
                        " flux or histogram counts was not positive definite." << endl;
                    break;
                }

                double difference =
                    combinedCSPrereqs[i].histoCounts-
                    (double)combinedCSPrereqs[j].histoCounts*
                    ((double)combinedCSPrereqs[i].BCICounts/
                     combinedCSPrereqs[j].BCICounts);

                // calculate differential cross section in lab frame
                cs.value = ((double)difference/REFERENCE_HISTO_COUNTS)*
                    ((double)reference.BCICounts/combinedCSPrereqs[i].BCICounts)*
                    (REFERENCE_NUMBER_OF_ATOMS/TARGET_NUMBER_OF_ATOMS)*
                    REFERENCE_CS;

                // convert lab frame cross section to center-of-mass frame via
                // Jacobian
                cs.value *= labToCMJacobian(labAngle, NEUTRON_MASS, TARGET_MASS);

                cs.error = 0;
                
                crossSections.push_back(cs);

                break;
            }

            if(j+1==combinedCSPrereqs.size())
            {
                cerr << "Error: didn't find blank target run for angle " <<
                    cs.angle << ", target " <<
                    combinedCSPrereqs[i].runConfig.targetName <<
                    "; cannot compute cross section." << endl;
                break;
            }
        }
    }

    outputCrossSections(crossSections, "Sn112", detectorName);
    outputCrossSections(crossSections, "Sn124", detectorName);

    return 0;
}
