#ifndef SENSITIVE_DETECTOR_HIT
#define SENSITIVE_DETECTOR_HIT

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

class SensitiveDetectorHit : public G4VHit
{
    public:

        SensitiveDetectorHit();
        ~SensitiveDetectorHit();
}

typedef G4THitsCollection<SensitiveDetectorHit> SensitiveDetectorHitCollection;
extern G4Allocator<SensitiveDetectorHit> SensitiveDetectorHitAllocator;

inline void* SensitiveDetectorHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) SensitiveDetectorHitAllocator.MallocSingle();
    return aHit;
}

inline void SensitiveDetectorHit::operator delete(void *aHit)
{
    SensitiveDetectorHitAllocator.FreeSingle((SensitiveDetectorHit*) aHit);
}

#endif /* SENSITIVE_DETECTOR_HIT */
