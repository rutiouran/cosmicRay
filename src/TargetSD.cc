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
{
  //Print();
}

void TargetSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new TargetHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );

  // Create hits
  for(G4int i=0; i<CopyNb; i++)
  {
    fHitsCollection->insert(new TargetHit());
  }
}

G4bool TargetSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
//  const G4VProcess* process = step->GetPostStepPoint()->GetProcessDefinedStep();

  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();

  // step length
  G4double steplength = 0.;
  if(step->GetTrack()->GetDefinition()->GetPDGCharge()!=0.)
  {
    steplength = step->GetStepLength();
  } 

  auto charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
  if (charge==0.) return true;
  //if(edep == 0. && steplength == 0.)  return false;

  G4int pid = step->GetTrack()->GetParticleDefinition()->GetPDGEncoding();
  G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();

  // the parameters
  G4double mutime = 0.; //mu arrives at the scintillator
  G4double electrontime = 0.; // decay

  // Get scintillator cell id (0 to 5)
  G4int layerNumber = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();

  auto hit = (*fHitsCollection)[layerNumber];
  if(!hit)
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hit " << layerNumber;
    G4Exception("B4cCalorimeterSD::ProcessHits()",
      "MyCode0004", FatalException, msg);
  }

  // Get the time when mu arrives at the scintillator 
  if(step->GetTrack()->GetTrackID()==1 && step->IsFirstStepInVolume())
  {
    mutime = step->GetTrack()->GetGlobalTime();
    hit->AddMuTime(mutime);
  }

  // Get eletron from dacay of mu
  if(step->GetTrack()->GetParentID()==1
     &&
     step->GetTrack()->GetCreatorProcess()->GetProcessName() == "Decay"
     &&
     particleName=="e+"
     &&
     step->IsFirstStepInVolume())
  {
    electrontime = step->GetTrack()->GetGlobalTime();
    hit->AddElectronTime(electrontime);
  }

  hit->AddEdep(edep);

  // Get the decay position
  if(step->GetTrack()->GetTrackID()==1 && step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName()=="Decay")
  {
    //G4cout << "Process = " << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
    G4cout << "Position = " << step->GetTrack()->GetPosition().z() << G4endl;
    hit->AddDecayPosition(step->GetTrack()->GetPosition().z());
  }

  return true;
  

//  G4String processName = process->GetProcessName();

  

//  TargetHit* newHit = new TargetHit();
//
//  newHit->SetTrackID(step->GetTrack()->GetTrackID());
//  newHit->SetTargetNb(step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber());
//  newHit->SetEdep(edep);
//  newHit->SetGlobalTime(step->GetTrack()->GetGlobalTime());
//  newHit->SetPid(step->GetTrack()->GetParticleDefinition()->GetPDGEncoding());
//  newHit->SetPolarization(step->GetTrack()->GetPolarization());
//  newHit->SetProcessName(processName);
//  fHitsCollection->insert(newHit);
//
//  if(step->GetTrack()->GetTrackID()==1
//     &&
//     step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber()==0
//     &&
//     step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()=="Target"
//     &&
//     step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "Envelope")
//  {
//  muEnergy = step->GetTrack()->GetKineticEnergy();
//  G4cout << "Target = " << muEnergy << G4endl;
//  }
//
//  if(step->GetTrack()->GetParentID()>0
//     &&
//     step->GetTrack()->GetCreatorProcess()->GetProcessName() == "Decay"
//     &&
//     particleName=="e+") // Get the dacay source of mu
//  {
//  GetDecayOfElectronic(step, muEnergy);
//  }
}

void TargetSD::EndOfEvent(G4HCofThisEvent*)
{}

//void TargetSD::GetDecayOfElectronic(G4Step* step, G4double muenergy)
//{
//  if(step->GetTrack()->GetCurrentStepNumber()==1) // initializing
//  {
//    firstNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();
//    lastNb = -1;
//    court = 0;
//  }
//
//  lastNb = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(); // Secend entry into scintlliator
//
//  decayZ = step->GetTrack()->GetPosition().z();
//
//  if(court==0)
//  {
//  if(firstNb != lastNb && firstNb!=-1 && lastNb!=-1)
//  {
//  if(firstNb<lastNb) 
//  {
//  downEvent(muenergy, decayZ); 
//  }
//  else
//  {
//  upEvent(muenergy, decayZ);
//  }
//
//  court++;
//  }
//  }
//}
//
//void TargetSD::upEvent(G4double de, G4double dz)
//{
//  auto analysisManager = G4AnalysisManager::Instance();
//  upCount++;
//  analysisManager->FillH1(0, de);
//  analysisManager->FillH1(2, dz);
//}
//
//void TargetSD::downEvent(G4double de, G4double dz)
//{
//  auto analysisManager = G4AnalysisManager::Instance();
//  downCount++;
//  analysisManager->FillH1(1, de);
//  analysisManager->FillH1(3, dz);
//}
