#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include <sstream>

#include "../include/Target.h"

using namespace std;

Target::Target(string targetDataLocation)
{
    string targetPhysicalData = targetDataLocation + "physical.txt";

    ifstream dataFile(targetPhysicalData.c_str());
    if(!dataFile.is_open())
    {
        std::cout << "Attempted to create Target, but failed to find target data in " << targetPhysicalData << std::endl;
        exit(1);
    }

    string str;

    while(getline(dataFile,str))
    {
        // parse into tokens

        vector<string> tokens;
        istringstream iss(str);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        if((tokens[0]=="Diameter") && (tokens[1] == "uncertainty:"))
        {
            diameterUncertainty = stod(tokens[2]);
        }

        else if((tokens[0]=="Mass") && (tokens[1] == "uncertainty:"))
        {
            massUncertainty = stod(tokens[2]);
        }

        else if((tokens[0]=="Molar") && (tokens[2] == "uncertainty:"))
        {
            molMassUncertainty = stod(tokens[3]);
        }

        else if(tokens[0]=="Name:")
        {
            name = tokens[1];
        }

        else if(tokens[0]=="Length:")
        {
            length = stod(tokens[1]);
        }

        else if(tokens[0]=="Diameter:")
        {
            diameter = stod(tokens[1]);
        }

        else if(tokens[0]=="Mass:")
        {
            mass = stod(tokens[1]);
        }

        else if(tokens[0]=="Molar")
        {
            molMass = stod(tokens[2]);
        }

        else if(tokens[0]=="Excited")
        {
            excitedStates.push_back(stod(tokens[2]));
        }

        else
        {
            cerr << "Error - couldn't parse a line in a targetData text file" << endl;
            exit(1);
        }
    }

    histos.push_back(vector<TH1D*>());
    histos.push_back(vector<TH1D*>());

    monitors.push_back(vector<TH1D*>());
    monitors.push_back(vector<TH1D*>());
}

string Target::getName() const
{
    return name;
}

double Target::getLength() const
{
    return length;
}

double Target::getDiameter() const
{
    return diameter;
}

double Target::getMass() const
{
    return mass;
}

double Target::getMolarMass() const
{
    return molMass;
}

double Target::getDiameterUncertainty() const
{
    return diameterUncertainty;
}

double Target::getMassUncertainty() const
{
    return massUncertainty;
}

double Target::getMolarMassUncertainty() const
{
    return molMassUncertainty;
}

void Target::setName(string n)
{
    name = n;
}

void Target::setLength(double l)
{
    length = l;
}

void Target::setDiameter(double d)
{
    diameter = d;
}

void Target::setMass(double m)
{
    mass = m;
}

void Target::setMolarMass(double mm)
{
    molMass = mm;
}

void Target::setDiameterUncertainty(double d)
{
    diameterUncertainty = d;
}

void Target::setMassUncertainty(double m)
{
    massUncertainty = m;
}

void Target::setMolarMassUncertainty(double mm)
{
    molMassUncertainty = mm;
}
