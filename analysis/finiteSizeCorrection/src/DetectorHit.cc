#include "DetectorHit.hh"

G4Allocator<DetectorHit> DetectorHitAllocator;

DetectorHit::DetectorHit() {}

DetectorHit::~DetectorHit() {}

DetectorHit::DetectorHit(const DetectorHit& right)
  : G4VHit()
{
  edep      = right.edep;
  pos       = right.pos;
  pname = right.pname;
  detno = right.detno;
  parentID = right.parentID;
  neng = right.neng;
  hit_time = right.hit_time;
}

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
  edep      = right.edep;
  pos       = right.pos;
  pname = right.pname;
  detno = right.detno;
  parentID = right.parentID;
  neng = right.neng;
  hit_time = right.hit_time;
  return *this;
}

G4int DetectorHit::operator==(const DetectorHit& right) const
{
    if(this==&right)
    {
        return 1;
    }
    
    return 0;
}

void DetectorHit::Draw() {}

void DetectorHit::Print()
{
  G4cout << "  energy deposit: " << G4BestUnit(edep,"Energy")
	 << "  position: " << G4BestUnit(pos,"Length") 
	 << G4endl;
}
