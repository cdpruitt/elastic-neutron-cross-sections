#ifndef DetectorConstruction_H
#define DetectorConstruction_H

#include "G4VSensitiveDetector.hh"
#include "globals.hh"
#include "G4TouchableHistory.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4String.hh"
#include "G4SDManager.hh"
#include "G4Tubs.hh"
#include "MaterialFactory.hh"
#include "DetectorHit.hh"
#include "neutronElastic_TrackInformation.hh"

class DetectorConstruction : public G4VSensitiveDetector
{
    public:

        DetectorConstruction(G4String name);
        ~DetectorConstruction();
        void Initialize(G4HCofThisEvent *HCoE);
        G4bool ProcessHits(G4Step *theStep,G4TouchableHistory *roHist);
        void EndOfEvent(G4HCofThisEvent *HCoE);

        G4LogicalVolume* GetADetector(void) { return scint_log; };
        float GetCellThickness(void) { return cellthickness; };

    private:

        G4Tubs* scint_solid;
        G4LogicalVolume* scint_log; 
        MaterialFactory* matfactory;

        void buildScintillator(); 

        DetectorHitsCollection* detectorCollection;
        float cellthickness;
};

#endif /* DetectorConstruction_H */
