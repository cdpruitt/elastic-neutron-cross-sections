#include "DetectorConstruction.hh"

using namespace CLHEP;

DetectorConstruction::DetectorConstruction(G4String name)
    :G4VSensitiveDetector(name)
{
    G4String HCName;
    collectionName.insert(HCName = "detectorCollection");
    matfactory = new MaterialFactory();

    buildScintillator();
}

DetectorConstruction::~DetectorConstruction() {}

void DetectorConstruction::Initialize(G4HCofThisEvent *HCoE)
{
    detectorCollection = new DetectorHitsCollection
        (SensitiveDetectorName,collectionName[0]); 
    static G4int HCID = -1;

    if(HCID<0)
    {
        HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }

    HCoE->AddHitsCollection( HCID, detectorCollection );
}

G4bool DetectorConstruction::ProcessHits(G4Step *theStep,G4TouchableHistory *roHist)
{
    G4double edep = theStep->GetTotalEnergyDeposit();

    if(edep==0.)
    {
        return false;
    }

    DetectorHit* newHit = new DetectorHit();
    newHit->SetEdep(edep);
    newHit->SetPos(theStep->GetPreStepPoint()->GetPosition());
    newHit->SetParticleName(theStep->GetTrack()->GetDefinition()->GetParticleName());
    newHit->SetDetNo(0);
    neutronElastic_TrackInformation* info = (neutronElastic_TrackInformation*)(theStep->GetTrack()->GetUserInformation());
    newHit->SetParentID(info->GetOriginalTrackID());
    newHit->SetNeng(info->GetOriginalEnergy());
    newHit->SetHitTime(theStep->GetPreStepPoint()->GetGlobalTime()); // may need to be PostStepPoint ?

    detectorCollection->insert(newHit);

    return true;
}

void DetectorConstruction::EndOfEvent(G4HCofThisEvent *HCoE) {}

void DetectorConstruction::buildScintillator()
{
    cellthickness = 5.08;

    G4double rmin = 0.0*cm;
    G4double rmax = 12.68*.5*cm;
    G4double length = cellthickness*cm;
    G4double phi =    0;
    G4double dphi = 360;
    scint_solid = new G4Tubs("scint_solid",rmin,rmax,length/2.0,phi,dphi); // div by 2.0 here since it is half thickness

    G4Material* mat = matfactory->GetMaterial("BC501A");
    scint_log = new G4LogicalVolume(scint_solid,mat,"scint_log");

    G4SDManager::GetSDMpointer()->AddNewDetector(this);
    scint_log->SetSensitiveDetector(this);
}
