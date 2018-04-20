#include "G4VSensitiveDetector.hh"
#include "G4String.hh"

class G4Step;

class mySensitiveDetector : public G4VSensitiveDetector
{
    public:
        mySensitiveDetector(G4String name);
        virtual ~mySensitiveDetector();

        virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
};
