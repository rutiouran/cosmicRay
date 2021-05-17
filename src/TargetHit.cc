#include "TargetHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator=0;

TargetHit::TargetHit()
 : G4VHit(),
   fTrackID(-1),
   fTargetNb(-1),
   fEdep(0.),
   fTime(0.),
   fPid(0),
   fPolar(G4ThreeVector()),
   fProcess("NULL")
{}

TargetHit::~TargetHit() 
{}

TargetHit::TargetHit(const TargetHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fTargetNb  = right.fTargetNb;
  fEdep      = right.fEdep;
  fTime      = right.fTime;
  fPid       = right.fPid;
  fPolar     = right.fPolar;
  fProcess   = right.fProcess;
}

const TargetHit& TargetHit::operator=(const TargetHit& right)
{
  fTrackID   = right.fTrackID;
  fTargetNb  = right.fTargetNb;
  fEdep      = right.fEdep;
  fTime      = right.fTime;
  fPid       = right.fPid;
  fPolar     = right.fPolar;
  fProcess   = right.fProcess;

  return *this;
}

G4bool TargetHit::operator==(const TargetHit& right) const
{
  return ( this == &right ) ? true : false;
}
