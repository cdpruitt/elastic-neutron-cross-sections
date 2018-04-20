#include "mySensitiveDetector.hh"
//#include "SensitiveDetectorHit.hh"

mySensitiveDetector::mySensitiveDetector(G4String name) :
    G4VSensitiveDetector::G4VSensitiveDetector(name) {}

mySensitiveDetector::~mySensitiveDetector() {}

G4bool mySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist)
{
    std::cout << "processed a hit in sensitive detector" << std::endl;

    //SensitiveDetectorHitCollection

    return true;
}

