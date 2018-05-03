//from http://geant4.slac.stanford.edu/Tips/event/1.html

#ifndef neutronElastic_TrackInformation_h
#define neutronElastic_TrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class neutronElastic_TrackInformation : public G4VUserTrackInformation 
{
  public:
    neutronElastic_TrackInformation();
    neutronElastic_TrackInformation(const G4Track* aTrack);
    neutronElastic_TrackInformation(const neutronElastic_TrackInformation* aTrackInfo);
    virtual ~neutronElastic_TrackInformation();
   
    inline void *operator new(size_t);
    inline void operator delete(void *aTrackInfo);
    inline int operator ==(const neutronElastic_TrackInformation& right) const
    {return (this==&right);}

    void Print() const;

  private:
    G4int                 originalTrackID;
    G4ParticleDefinition* particleDefinition;
    G4ThreeVector         originalPosition;
    G4ThreeVector         originalMomentum;
    G4double              originalEnergy;
    G4double              originalTime;

  public:
    inline G4int GetOriginalTrackID() const {return originalTrackID;}
    inline G4ParticleDefinition* GetOriginalParticle() const {return particleDefinition;}
    inline G4ThreeVector GetOriginalPosition() const {return originalPosition;}
    inline G4ThreeVector GetOriginalMomentum() const {return originalMomentum;}
    inline G4double GetOriginalEnergy() const {return originalEnergy;}
    inline G4double GetOriginalTime() const {return originalTime;}
};

extern G4Allocator<neutronElastic_TrackInformation> aTrackInformationAllocator;

inline void* neutronElastic_TrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void neutronElastic_TrackInformation::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator.FreeSingle((neutronElastic_TrackInformation*)aTrackInfo);}

#endif
