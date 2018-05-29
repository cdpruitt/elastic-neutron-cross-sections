#include "G4VSensitiveDetector.hh"
#include "G4String.hh"

#include "PerfectDetHit.hh"
#include "DetectorDataTree.hh"

class G4Step;

class mySensitiveDetector : public G4VSensitiveDetector
{
    public:
        mySensitiveDetector(G4String name);
        ~mySensitiveDetector();

        void Initialize(G4HCofThisEvent *HCE);
        G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
        void EndOfEvent(G4HCofThisEvent*);

    private:
        PerfectDetHitsCollection* DetectorHitsCollection;
        DetectorDataTree* detectorDataTree;
};
