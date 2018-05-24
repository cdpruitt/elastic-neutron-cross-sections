#ifndef DATARECORD_H
#define DATARECORD_H

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include <iostream>

// C++ formatting and file io headers

#include <iomanip>
#include <fstream>
#include <cmath>

// Root Analysis header files for C++

#include "Riostream.h"
#include "TFile.h"
#include "TH1.h"
#include "TNtuple.h"
#include "TTree.h"
#include "globals.hh"
#include "G4UnitsTable.hh"

#include "G4ThreeVector.hh"

class DetectorDataTree
{
    public:

        // Initialized in class constructor in DetectorDataTree.cc 

        TFile* file;
        TTree* tree;

        G4double eng_int;
        G4double theta_int;
        G4double phi_int;
        G4ThreeVector theStartPos;

        G4double KEOfEvent;
        G4double TOFOfEvent;
        G4double PathLengthEvent;

        // Particle Counters

        int event_counter;
        int number_detected;

        DetectorDataTree();
        ~DetectorDataTree();

        // Creates Pointer to Data Analysis in main() program (Margot.cc).    
        static DetectorDataTree* pointer;

        // Definition of member functions - access and distribute data
        // See "DetectorDataTree.cc for the function listings.

        void senddataPG(G4double value1, G4ThreeVector theStartPos);
        void ShowDataFromEvent();
        void GetParticleTotals();
        void FillTree();
}; 

#endif
