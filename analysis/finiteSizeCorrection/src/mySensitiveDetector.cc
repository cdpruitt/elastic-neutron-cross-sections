#include "PerfectDetHit.hh"
#include "mySensitiveDetector.hh"

#include "G4VProcess.hh"
#include "G4SDManager.hh"

using namespace std;

mySensitiveDetector::mySensitiveDetector(G4String name) :
    G4VSensitiveDetector(name)
{
    G4String HCname;
    collectionName.insert(HCname="DetectorHitsCollection");
    detectorDataTree = DetectorDataTree::pointer;
}

mySensitiveDetector::~mySensitiveDetector() {}

void mySensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
    DetectorHitsCollection = new PerfectDetHitsCollection(SensitiveDetectorName,collectionName[0]);

    static int DetCollectionID = -1;

    if(DetCollectionID<0)
    {
        DetCollectionID = GetCollectionID(0);
    }

    HCE->AddHitsCollection(GetCollectionID(0), DetectorHitsCollection);

    //The below is for debugging purposes
   //G4cout << "DetCollection ID = " << DetCollectionID << G4endl;
   //G4cout << "The Detector was initialized!" << G4endl;
   //G4cout << "This is the address of the  ->HitsCollection (HCE) : " << HCE << G4endl;
}

G4bool mySensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    // Creates a Hit if EnergyDep > 0!

    //G4double edep = aStep->GetTotalEnergyDeposit();

    /*if(!edep)
    {
        return false;
    }*/

    PerfectDetHit* newHit = new PerfectDetHit();

    newHit->SetPos( aStep->GetPostStepPoint()->GetPosition() ); // Note - collects reaction point! 

    G4double theTime = aStep->GetTrack()->GetGlobalTime();
    newHit->SetTOF(theTime);

    newHit->SetParticleName( aStep->GetTrack()->GetDefinition()->GetParticleName() );

    newHit->SetDetCopyNumber(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0));
    newHit->SetDetName(aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(0)->GetName());

    const G4VProcess* theProcess = aStep->GetTrack()->GetCreatorProcess();

    if(theProcess != 0)
    {
        const G4String theProcessName = theProcess->GetProcessName();
        newHit->SetParticleProcess(theProcessName);
    }

    else
    {
        newHit->SetParticleProcess("NoReaction");
    }

    DetectorHitsCollection->insert(newHit);

    return true;
}

void mySensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();
    G4String detName = "scorer";
    G4int collectionID = SDMan->GetCollectionID("scorer/DetectorHitsCollection");

    PerfectDetHitsCollection* myCollection = (PerfectDetHitsCollection*)(HCE->GetHC(collectionID));

    G4ThreeVector thePos(0.0,0.0,0.0);
    G4String theParticleName = "none";
    G4String theDetName = "none";
    G4int theDetCopyNum = -1;
    G4double KinEngDep = 0.;

    if(myCollection)
    {
        int n_hit = myCollection->entries();

        if(n_hit>0)
        {
            PerfectDetHit* theFirstHit = (*myCollection)[0];

            theDetName = theFirstHit->GetDetName(); 
            theDetCopyNum = theFirstHit->GetDetCopyNumber();
            theParticleName = theFirstHit->GetParticleName();

            detectorDataTree->KEOfEvent = theFirstHit->GetKinEng();
            thePos = theFirstHit->GetPos();
            detectorDataTree->TOFOfEvent = theFirstHit->GetTOF();


            //if(theFirstHit->GetParticleProcess()=="NoReaction")
            {
                detectorDataTree->FillTree();
                detectorDataTree->TOFHisto->Fill(detectorDataTree->TOFOfEvent);

                if(theParticleName=="neutron")
                {
                    detectorDataTree->neutronTOFHisto->Fill(detectorDataTree->TOFOfEvent);
                }
            }
        }

        // Event readout loop!

        /*for(int i=0;i<n_hit;i++)
        {
            // (*myCollection)[i] is the pointer to the i-th hit of the event.
            // All data analysis output is read out here and then sent to the DataTree!

            PerfectDetHit* theCurrentHit = (*myCollection)[i];

            theDetName = theCurrentHit->GetDetName(); 
            theDetCopyNum = theCurrentHit->GetDetCopyNumber();
            theParticleName = theCurrentHit->GetParticleName();

            detectorDataTree->KEOfEvent = theCurrentHit->GetKinEng();
            thePos = theCurrentHit->GetPos();
            detectorDataTree->TOFOfEvent = theCurrentHit->GetTOF();
        }*/
    }

    else
    {
        /* No hit collection! (although, always a HitCollection even if no Hits ! */
        G4cout << "Warning! No Hits Collection! " << G4endl;
    }


}
