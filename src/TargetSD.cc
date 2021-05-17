#include "TargetSD.hh"
#include "DetectorConstruction.hh"
#include "G4VProcess.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "g4root.hh"

TargetSD::TargetSD(const G4String& name,
                   const G4String& hitsCollectionName)
 : G4VSensitiveDetector(name),
   fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
  CopyNb = DetectorConstruction::GetCopyNb();
}

TargetSD::~TargetSD() 
{}

void TargetSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new TargetHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );

  for (G4int i=0; i<CopyNb+1; i++)
  {
    fHitsCollection->insert(new TargetHit());
  }
}

G4bool TargetSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  const G4VProcess* process = step->GetPostStepPoint()->GetProcessDefinedStep();

  //G4int pid = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();

  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  if(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding()!=-14 && edep==0.)
  return false;

  G4String processName = process->GetProcessName();

  if(processName == "Decay")
  {
    G4cout << "1" << G4endl;
  }

  TargetHit* newHit = new TargetHit();

  newHit->SetTrackID(step->GetTrack()->GetTrackID());
  newHit->SetTargetNb(step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
  newHit->SetEdep(edep);
  newHit->SetGlobalTime(step->GetTrack()->GetGlobalTime());
  newHit->SetPid(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
  newHit->SetPolarization(step->GetTrack()->GetPolarization());
  //newHit->SetProcessName(processName);
  fHitsCollection->insert( newHit );

  return true;
}

void TargetSD::EndOfEvent(G4HCofThisEvent*)
{}
