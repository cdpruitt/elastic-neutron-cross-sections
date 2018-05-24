
#ifndef pf_ExperimentConstruction_H
#define pf_ExperimentConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "G4ios.hh"
#include <string>
#include <cstdlib>
#include <vector>
#include <csignal>
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "MaterialFactory.hh"
#include "DetectorConstruction.hh"
#include "G4UserLimits.hh"
#include "G4VSensitiveDetector.hh"

#include "G4SystemOfUnits.hh" // including this header is a new requirement for GEANT-defined units for GEANT 10.0+

using namespace std;

const G4double DET_DISTANCE = 37.5*cm;
const G4double DET_ANGLE = 30*deg;

const G4double ORIGIN_OFFSET = 0*cm;

class neutronElastic_ExperimentConstruction :
    public G4VUserDetectorConstruction
{
    public:

        neutronElastic_ExperimentConstruction();
        ~neutronElastic_ExperimentConstruction();

        G4VPhysicalVolume* Construct();

        virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROHist);

        //inline vector<double> GetBackFaceDist() {return back_face_dist;}

    private:

        // Logical volumes
        G4LogicalVolume* worldVolume_log;

        // Physical volumes
        G4VPhysicalVolume* worldVolume_phys;

        DetectorConstruction* detConstruction;

        MaterialFactory* materialFactory;

        string dir_for_det_info;

        //vector<double> back_face_dist;
};

#endif

