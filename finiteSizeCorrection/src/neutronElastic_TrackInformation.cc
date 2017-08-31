//from http://geant4.slac.stanford.edu/Tips/event/1.html

#include "neutronElastic_TrackInformation.hh"
#include "G4ios.hh"

G4Allocator<neutronElastic_TrackInformation> aTrackInformationAllocator;

neutronElastic_TrackInformation::neutronElastic_TrackInformation()
{
    originalTrackID = 0;
    particleDefinition = 0;
    originalPosition = G4ThreeVector(0.,0.,0.);
    originalMomentum = G4ThreeVector(0.,0.,0.);
    originalEnergy = 0.;
    originalTime = 0.;
}

neutronElastic_TrackInformation::neutronElastic_TrackInformation(const G4Track* aTrack)
{
    originalTrackID = aTrack->GetTrackID();
    particleDefinition = aTrack->GetDefinition();
    originalPosition = aTrack->GetPosition();
    originalMomentum = aTrack->GetMomentum();
    //originalEnergy = aTrack->GetTotalEnergy();
    originalEnergy = aTrack->GetKineticEnergy();
    originalTime = aTrack->GetGlobalTime();
}

neutronElastic_TrackInformation::neutronElastic_TrackInformation(const neutronElastic_TrackInformation* aTrackInfo)
{
    originalTrackID = aTrackInfo->originalTrackID;
    particleDefinition = aTrackInfo->particleDefinition;
    originalPosition = aTrackInfo->originalPosition;
    originalMomentum = aTrackInfo->originalMomentum;
    originalEnergy = aTrackInfo->originalEnergy;
    originalTime = aTrackInfo->originalTime;
}

neutronElastic_TrackInformation::~neutronElastic_TrackInformation(){;}

void neutronElastic_TrackInformation::Print() const
{
    G4cout 
     << "Original track ID " << originalTrackID 
     << " at " << originalPosition << G4endl;
}
