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

int main(int argc, char** argv)
{
    string inputFileName = argv[1];
    double scalingFactor = atof(argv[2]);
    string outputFileName = argv[3];

    // read in data from input file and store each datapoint in data vector
    vector<ElasticCSDataPoint> data;

    ifstream inputFile(inputFileName);

    if(!inputFile.is_open())
    {
        cerr << "Error: failed to open " << inputFileName << ". Exiting..." << endl;
    }

    string dummy1;
    string dummy2;
    string dummy3;

    // read first two lines (data labels) for copying into output file
    string firstLine;
    string secondLine;
    string thirdLine;

    getline(inputFile,firstLine);
    getline(inputFile,secondLine);
    getline(inputFile,thirdLine);

    while(inputFile >> dummy1 >> dummy2 >> dummy3)
    {
        data.push_back(ElasticCSDataPoint(stod(dummy1), stod(dummy2), stod(dummy3)));
    }

    ofstream outputFile(outputFileName);

    outputFile << firstLine << endl;
    outputFile << secondLine << endl;
    outputFile << thirdLine << endl;

    for(ElasticCSDataPoint dp : data)
    {
        outputFile << right << setw(8) << dp.degrees;
        outputFile << setw(10) << dp.elasticCS*scalingFactor;
        outputFile << setw(10) << dp.error*scalingFactor;
        outputFile << endl;
    }

    return 0;
}
