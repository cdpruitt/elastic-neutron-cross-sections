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
#include "../include/RunConfig.h"

using namespace std;

const double REFERENCE_CS = 163.8; // p,n cross section in mb/sr - for lab angle of 23 degrees @ 17 MeV
const double REFERENCE_NUMBER_OF_ATOMS = 2.91e23;
const double Sn112_NUMBER_OF_ATOMS = 2.676e22;
const double Sn124_NUMBER_OF_ATOMS = 2.697e22;

double TARGET_NUMBER_OF_ATOMS;
double TARGET_MASS;

const double REFERENCE_4M_COUNTS = 12308;
const double REFERENCE_6M_COUNTS = 11784;
const double REFERENCE_MONITOR_COUNTS = 88910;

double REFERENCE_HISTO_COUNTS; 

const double FOURM_INT_LIMIT_MIN = 1276;
const double FOURM_INT_LIMIT_MAX = 1302;
double INTEGRAL_LIMIT_MIN;

const double SIXM_INT_LIMIT_MIN = 940;
const double SIXM_INT_LIMIT_MAX = 975;
double INTEGRAL_LIMIT_MAX;

const double NEUTRON_MASS = 1.008665; // in unified atomic mass units

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

/*vector<unsigned int> getIntegralBounds(string integralBoundsFileName)
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
}*/

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

int main()
{
    vector<string> detectorNames = {"4M", "6M"};
    vector<string> targetNames = {"Sn112", "Sn124"};

    AllConfigs allConfigs = getRunConfig();

    AllAngles allAngles;
    allAngles.getRunData(allConfigs);

    ofstream Sn112_4M("literatureData/Sn112_4M.txt");
    ofstream Sn124_4M("literatureData/Sn124_4M.txt");
    ofstream Sn112_6M("literatureData/Sn112_6M.txt");
    ofstream Sn124_6M("literatureData/Sn124_6M.txt");

    Sn112_4M << endl << "Sn112, 4M" << endl;
    Sn112_4M << "Degrees    mB/sr      Error (mB/sr)" << endl;

    Sn124_4M << endl << "Sn124, 4M" << endl;
    Sn124_4M << "Degrees    mB/sr      Error (mB/sr)" << endl;

    Sn112_6M << endl << "Sn112, 6M" << endl;
    Sn112_6M << "Degrees    mB/sr      Error (mB/sr)" << endl;

    Sn124_6M << endl << "Sn124, 6M" << endl;
    Sn124_6M << "Degrees    mB/sr      Error (mB/sr)" << endl;

    ofstream* fileOut;

    for(auto& target : targetNames)
    {
        if(target=="Sn112")
        {
            TARGET_NUMBER_OF_ATOMS = Sn112_NUMBER_OF_ATOMS; 
            TARGET_MASS = 111.905; // in unified atomic mass units
        }

        else if (target=="Sn124")
        {
            TARGET_NUMBER_OF_ATOMS = Sn124_NUMBER_OF_ATOMS; 
            TARGET_MASS = 123.905; // in unified atomic mass units
        }

        for(auto& detectorName : detectorNames)
        {
            for(auto& angle : allAngles.angles)
            {
                if(detectorName=="4M")
                {
                    REFERENCE_HISTO_COUNTS = REFERENCE_4M_COUNTS;
                    INTEGRAL_LIMIT_MIN = FOURM_INT_LIMIT_MIN;
                    INTEGRAL_LIMIT_MAX = FOURM_INT_LIMIT_MAX;

                    if(target=="Sn112")
                    {
                        fileOut = &Sn112_4M;
                    }

                    else if(target=="Sn124")
                    {
                        fileOut = &Sn124_4M;
                    }
                }

                else if(detectorName=="6M")
                {
                    REFERENCE_HISTO_COUNTS = REFERENCE_6M_COUNTS;
                    INTEGRAL_LIMIT_MIN = SIXM_INT_LIMIT_MIN;
                    INTEGRAL_LIMIT_MAX = SIXM_INT_LIMIT_MAX;

                    if(target=="Sn112")
                    {
                        fileOut = &Sn112_6M;
                    }

                    else if(target=="Sn124")
                    {
                        fileOut = &Sn124_6M;
                    }
                }

                // read histogram counts for each run
                stringstream ss;
                ss << setprecision(5) << angle.angle;
                string histoFileName = "../processedData/angles/" + ss.str() + "/" + target + ".root";
                TFile histoFile(histoFileName.c_str(),"UPDATE");

                if(!histoFile.IsOpen())
                {
                    continue;
                }

                string histoName = "diff" + detectorName;
                TH1I* histo = (TH1I*)histoFile.Get(histoName.c_str());

                if(!histo)
                {
                    continue;
                }

                // extract counts from histo based on histogram gates
                int minIntBin = histo->FindBin(INTEGRAL_LIMIT_MIN);
                int maxIntBin = histo->FindBin(INTEGRAL_LIMIT_MAX);

                double difference = histo->Integral(minIntBin, maxIntBin);

                // convert from lab angle to CM angle
                double CMAngle = labAngleToCMAngle(angle.angle, NEUTRON_MASS, TARGET_MASS);

                // calculate differential cross section in lab frame
                double value = ((double)difference/REFERENCE_HISTO_COUNTS)*
                    ((double)REFERENCE_MONITOR_COUNTS/NORMALIZATION_SCALING)*
                    (REFERENCE_NUMBER_OF_ATOMS/TARGET_NUMBER_OF_ATOMS)*
                    REFERENCE_CS;

                // convert lab frame cross section to center-of-mass frame via
                // Jacobian
                value *= labToCMJacobian(angle.angle, NEUTRON_MASS, TARGET_MASS);

                *fileOut << angle.angle << " " << value
                    << " " << "0" << endl;

                // need to add efficiency correction for energy drop at high angles affecting count
                // rates
            }
        }
    }

    return 0;
}
