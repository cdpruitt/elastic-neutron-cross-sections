#ifndef neutronElastic_EventAction_h
#define neutronElastic_EventAction_h 1

#include "G4UserEventAction.hh"
#include "DetectorHit.hh"
#include "G4SDManager.hh" 
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"
#include <algorithm>

class neutronElastic_EventAction : public G4UserEventAction
{
  public:
  neutronElastic_EventAction();
   ~neutronElastic_EventAction();
    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);
  void resetVectors();

private:
  G4HCofThisEvent *fHCoE;
  DetectorHitsCollection *fHitsCollection;
  G4int fCollectionID;
  std::vector<float> curEdep;
  std::vector<float> curLo;
  std::vector<float> curNeng;
  std::vector<float> earliestHitTime;
  std::vector<float> biggestLOHitTime;
  std::vector<float> biggestLO;
  std::vector<float> curHitTime;
  std::vector<int> parentPrimaryID;
  std::vector<float> xint;
  std::vector<float> yint;
  std::vector<float> zint;
  int ndet;
};

#endif
