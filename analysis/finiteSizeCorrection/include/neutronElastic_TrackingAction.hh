#ifndef neutronElastic_TrackingAction_h
#define neutronElastic_TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "DetectorHit.hh"
#include "G4Event.hh"
#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "globals.hh"

class neutronElastic_TrackingAction : public G4UserTrackingAction
{
  public:
  neutronElastic_TrackingAction();
  virtual ~neutronElastic_TrackingAction();
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);
  
private:
  //G4TrackingManager* fpTrackingManager;

};

#endif
