#ifndef DetectorHit_h
#define DetectorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4UnitsTable.hh"

class DetectorHit : public G4VHit
{
    public:

        DetectorHit();
        ~DetectorHit();
        DetectorHit(const DetectorHit &right);
        const DetectorHit& operator=(const DetectorHit &right);
        int operator==(const DetectorHit &right) const;

        inline void * operator new(size_t);
        inline void operator delete(void *aHit);

        void Draw();
        void Print();

    private:

        G4double edep;
        G4ThreeVector pos;
        G4String pname;
        G4int detno;
        G4int parentID;
        G4double neng;
        G4double hit_time;

    public:

        inline void SetEdep(G4double de)
        {
            edep = de;
        }

        inline G4double GetEdep() const
        {
            return edep;
        }

        inline void SetPos(G4ThreeVector xyz)
        {
            pos = xyz;
        }

        inline G4ThreeVector GetPos() const
        {
            return pos;
        }

        inline void SetDetNo(G4int detno2)
        {
            detno = detno2;
        }

        inline G4int GetDetNo() const
        {
            return detno;
        }

        inline void SetParticleName(G4String pname2)
        {
            pname = pname2;
        }

        inline G4String GetParticleName() const
        {
            return pname;
        }

        inline void SetParentID(G4int id) 
        {
            parentID = id;
        }

        inline G4int GetParentID()
        {
            return parentID;
        }

        inline void SetNeng(G4double neng2) 
        {
            neng = neng2;
        }

        inline G4double GetNeng()
        {
            return neng;
        }

        inline void SetHitTime(G4double hit_time2) 
        {
            hit_time = hit_time2;
        }

        inline G4double GetHitTime()
        {
            return hit_time;
        }
};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

extern G4Allocator<DetectorHit> DetectorHitAllocator;

inline void* DetectorHit::operator new(size_t)
{
    void *aHit;
    aHit = (void *) DetectorHitAllocator.MallocSingle();
    return aHit;
}

inline void DetectorHit::operator delete(void *aHit)
{
    DetectorHitAllocator.FreeSingle((DetectorHit*) aHit);
}

#endif
