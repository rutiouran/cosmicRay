#ifndef TargetHit_h
#define TargetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4VProcess.hh"
#include "tls.hh"

class TargetHit : public G4VHit
{
  public:
    TargetHit();
    TargetHit(const TargetHit&);
    virtual ~TargetHit();

    // operators
    const TargetHit& operator=(const TargetHit&);
    G4bool operator==(const TargetHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // Add values
    void AddEdep(G4double de);
    void AddMuTime(G4double mutime);
    void AddElectronTime(G4double electrontime);
    void AddDecayPosition(G4double decayPosition);

    // Get methods
    G4double GetEdep() const {return fEdep;};
    G4double GetMuTime() const {return fmutime;};
    G4double GetElectronTime() const {return felectiontime;};
    G4double GetDecayPosition() const {return fDecayPosition;};

  private:
    G4double fEdep;
    G4double fmutime;
    G4double felectiontime;
    G4double fDecayPosition;
};

typedef G4THitsCollection<TargetHit> TargetHitsCollection;

extern G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator;

inline void* TargetHit::operator new(size_t)
{
  if(!TargetHitAllocator)
      TargetHitAllocator = new G4Allocator<TargetHit>;
  return (void *) TargetHitAllocator->MallocSingle();
}

inline void TargetHit::operator delete(void *hit)
{
  TargetHitAllocator->FreeSingle((TargetHit*) hit);
}

#endif
