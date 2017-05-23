#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct ElasticCSDataPoint
{
    ElasticCSDataPoint(double d, double ecs, double e) : degrees(d), elasticCS(ecs), error(e) {}

    double degrees;
    double elasticCS;
    double error;
};

struct ExpDataPoint
{
    unsigned int Det4M = 0;
    unsigned int Det6M = 0;
    unsigned int Mon0Degree = 0;
    unsigned int MonCeiling = 0;

    double TargetAtoms = 0;
};

int main(int argc, char** argv)
{
    string inputFileName = argv[1];

    inputFileName = "rawExpData/" + inputFileName;
    string outputFileName = "processsedExpData/" + inputFileName;

    string controlFileName = "rawExpData/polyethylene.data";
    ifstream controlFile(controlFileName.c_str());

    if(!controlFile.is_open())
    {
        cerr << "Error: failed to open " << controlFileName << ". Exiting..." << endl;
        exit(1);
    }

    // read in data from control file
    ExpDataPoint controlDataPoint;
    double controlCS;

    string label;
    string value;

    while(controlFile >> label >> value)
    {
        if(label=="Det4M")
        {
            controlDataPoint.Det4M = stod(value);
        }

        else if(label=="Det6M")
        {
            controlDataPoint.Det6M = stod(value);
        }

        else if(label=="Mon0Degree")
        {
            controlDataPoint.Mon0Degree = stod(value);
        }

        else if(label=="MonCeiling")
        {
            controlDataPoint.MonCeiling = stod(value);
        }

        else if(label=="TargetAtoms")
        {
            controlDataPoint.TargetAtoms = stod(value);
        }

        else if(label=="ControlCS")
        {
            controlCS = stod(value);
        }
    }

    // read in data from experimental data file
    ExpDataPoint expDataPoint;

    double angle;
    double energy;

    ifstream inputFile(inputFileName);

    if(!inputFile.is_open())
    {
        cerr << "Error: failed to open " << inputFileName << ". Exiting..." << endl;
        exit(1);
    }

    while(inputFile >> label >> value)
    {
        if(label=="Det4M")
        {
            expDataPoint.Det4M = stod(value);
        }

        else if(label=="Det6M")
        {
            expDataPoint.Det6M = stod(value);
        }

        else if(label=="Mon0Degree")
        {
            expDataPoint.Mon0Degree = stod(value);
        }

        else if(label=="MonCeiling")
        {
            expDataPoint.MonCeiling = stod(value);
        }

        else if(label=="TargetAtoms")
        {
            expDataPoint.TargetAtoms = stod(value);
        }

        else if(label=="Angle")
        {
            angle = stod(value);
        }

        else if(label=="Energy")
        {
            energy = stod(value);
        }
    }

    // calculate normalized cross section for experimental data point

    cout << "expDataPoint.Det4M = " << expDataPoint.Det4M << endl;
    cout << "controlDataPoint.Det4M = " << controlDataPoint.Det4M << endl;

    cout << "expDataPoint.Det6M = " << expDataPoint.Det6M << endl;
    cout << "controlDataPoint.Det6M = " << controlDataPoint.Det6M << endl;

    cout << "expDataPoint.MonCeiling = " << expDataPoint.MonCeiling << endl;
    cout << "controlDataPoint.MonCeiling = " << controlDataPoint.MonCeiling << endl;

    cout << "expDataPoint.TargetAtoms = " << expDataPoint.TargetAtoms << endl;
    cout << "controlDataPoint.TargetAtoms = " << controlDataPoint.TargetAtoms << endl;

    cout << "controlCS = " << controlCS << endl;

    double crossSection4M =
        ((double)expDataPoint.Det4M/controlDataPoint.Det4M)*
        ((double)controlDataPoint.MonCeiling/expDataPoint.MonCeiling)*
        ((double)controlDataPoint.TargetAtoms/expDataPoint.TargetAtoms)*
        controlCS;

    double crossSection6M =
        ((double)expDataPoint.Det6M/controlDataPoint.Det6M)*
        ((double)controlDataPoint.MonCeiling/expDataPoint.MonCeiling)*
        ((double)controlDataPoint.TargetAtoms/expDataPoint.TargetAtoms)*
        controlCS;


    cout << "4 M calculated CS = " << crossSection4M << endl;
    cout << "6 M calculated CS = " << crossSection6M << endl;

    ofstream outputFile(outputFileName);

    outputFile << "Angle   " << "elasticCS (4M)" << endl;
    outputFile << angle << "   " << crossSection4M << endl;

    outputFile << endl;
    outputFile << "Angle   " << "elasticCS (6M)" << endl;
    outputFile << angle << "   " << crossSection6M;


    return 0;
}
