//==========================================================================
// PerfectDetHit.cc
// Based on DemonScintHit.cc and ExN04TrackerHit.cc
//
// Written/Modified by: Brian Roeder, LPC Caen 08/18/07
//                      email - roeder@lpccaen.in2p3.fr
//
// Purpose: Defines PerfectDetHit class for simulation data readout
//          Sets and accesses Data in PerfectDetHit.hh methods
//
//==========================================================================
//
// - See UM Hits Presentation and JLab Scoring 2 Talk for more info.
//

#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "PerfectDetHit.hh"

G4Allocator<PerfectDetHit> PerfectDetHitAllocator;


PerfectDetHit::PerfectDetHit() 
{ /* Constructor */ }


PerfectDetHit::~PerfectDetHit() 
{ /* Destructor */ }


// The below taken from ExNO4TrackerHit.cc, but is not needed!
/*
PerfectDetHit::PerfectDetHit(const PerfectDetHit &right) : G4VHit()
{
  G4cout << ">>>>>>>>>>>>>>>>>>>>>A Hit was seen" << G4endl;
  edep = right.edep;
  pos = right.pos;
}

const PerfectDetHit& PerfectDetHit::operator=(const PerfectDetHit &right)
{
  edep = right.edep;
  pos = right.pos;
  return *this;
}

G4int PerfectDetHit::operator==(const PerfectDetHit &right) const
{
  return (this==&right) ? 1 : 0;
}
*/

// These Methods were taken directly from ExN04 and Demon

void PerfectDetHit::Draw()
{
  
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(10.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PerfectDetHit::Print()
{ /* Empty for now */ }
