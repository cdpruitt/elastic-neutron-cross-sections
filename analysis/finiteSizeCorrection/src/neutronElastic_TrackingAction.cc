//from http://geant4.slac.stanford.edu/Tips/event/1.html

#include "neutronElastic_TrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "neutronElastic_TrackInformation.hh"

neutronElastic_TrackingAction::neutronElastic_TrackingAction()
{}

neutronElastic_TrackingAction::~neutronElastic_TrackingAction()
{}

void neutronElastic_TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  if(aTrack->GetParentID()==0 && aTrack->GetUserInformation()==0)
  {
    neutronElastic_TrackInformation* anInfo = new neutronElastic_TrackInformation(aTrack);
    G4Track* theTrack = (G4Track*)aTrack;
    theTrack->SetUserInformation(anInfo);
  }
}

void neutronElastic_TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();

  if(secondaries)
  {
    neutronElastic_TrackInformation* info = (neutronElastic_TrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0; i<nSeco; i++)
      { 
        neutronElastic_TrackInformation* infoNew = new neutronElastic_TrackInformation(info);
        (*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
}
