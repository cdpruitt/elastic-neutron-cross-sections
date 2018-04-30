#include "../include/DiffGroup.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

vector<DiffGroup> readDiffHistoConfig()
{
    ifstream configFile("configuration/diffHistoConfig.txt");

    if(!configFile.is_open())
    {
        cerr << "Failed to find diff histo config file at configuration/diffHistoConfig.txt" << std::endl;
        exit(1);
    }

    string str;

    vector<DiffGroup> allGroups;

    while(getline(configFile,str))
    {
        // ignore comments in data file
        string delimiter = "-";
        string token = str.substr(0,str.find(delimiter));
        if(!atoi(token.c_str()))
        {
            // This line starts with a non-integer and is thus a comment; ignore
            continue;
        }

        DiffGroup group;
        stringstream(str) >> group.groupNumber >> group.angle4M >> group.angle6M >> group.blank >> group.target1 >> group.target2;

        allGroups.push_back(group);
    }

    return allGroups;
}

DiffGroup getDiffGroup(int groupNumber)
{
    ifstream configFile("configuration/diffHistoConfig.txt");

    if(!configFile.is_open())
    {
        cerr << "Failed to find diff histo config file at configuration/diffHistoConfig.txt" << std::endl;
        exit(1);
    }

    string str;

    vector<DiffGroup> allGroups;

    while(getline(configFile,str))
    {
        // ignore comments in data file
        string delimiter = "-";
        string token = str.substr(0,str.find(delimiter));
        if(!atoi(token.c_str()))
        {
            // This line starts with a non-integer and is thus a comment; ignore
            continue;
        }

        DiffGroup group;
        stringstream(str) >> group.groupNumber >> group.angle4M >> group.angle6M >> group.blank >> group.target1 >> group.target2;

        if(groupNumber==group.groupNumber)
        {
            return group;
        }
    }

    return DiffGroup();
}
