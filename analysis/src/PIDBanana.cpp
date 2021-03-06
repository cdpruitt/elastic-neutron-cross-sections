#include "../include/PIDBanana.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

PIDBanana::PIDBanana(string fileName)
{
    ifstream bananaFile(fileName.c_str());
    if(!bananaFile.is_open())
    {
        cerr << "Error: couldn't open banana file " + fileName + "." << endl;
        return;
    }

    string line;

    while(getline(bananaFile, line))
    {
        vector<string> tokens;
        istringstream iss(line);
        copy(istream_iterator<string>(iss),
                istream_iterator<string>(),
                back_inserter(tokens));

        X.push_back(stod(tokens[0]));
        Y.push_back(stod(tokens[1]));
    }
}
