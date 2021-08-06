#include "TargetHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator=0;

TargetHit::TargetHit()
 : G4VHit(),
   fEdep(0.),
   fmutime(0.),
   felectiontime(0.)
{}

TargetHit::~TargetHit() 
{}

TargetHit::TargetHit(const TargetHit& right)
  : G4VHit()
{
  fEdep         = right.fEdep;
  fmutime       = right.fmutime;
  felectiontime = right.felectiontime;
}

const TargetHit& TargetHit::operator=(const TargetHit& right)
{
  fEdep         = right.fEdep;
  fmutime       = right.fmutime;
  felectiontime = right.felectiontime;

  return *this;
}

G4bool TargetHit::operator==(const TargetHit& right) const
{
  return ( this == &right ) ? true : false;
}

void TargetHit::AddEdep(G4double de)
{
  fEdep+=de;
}

void TargetHit::AddMuTime(G4double mutime)
{
  fmutime = mutime;
}

void TargetHit::AddElectronTime(G4double electrontime)
{
  felectiontime = electrontime;
}
