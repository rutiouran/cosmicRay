#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "g4root.hh"
#include "G4RunManager.hh"
#include "G4PolarizationHelper.hh"

SteppingAction::SteppingAction(DetectorConstruction* det,
                               PrimaryGeneratorAction* prim, EventAction* eventAction)
 : G4UserSteppingAction(),
   fDetector(det), fPrimary(prim), fEvent(eventAction)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4StepPoint* prePoint = step->GetPreStepPoint();
  G4StepPoint* endPoint = step->GetPostStepPoint();

  G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  G4String processName = endPoint->GetProcessDefinedStep()->GetProcessName(); // Get process name of the step
  //G4String creatorProcessName = step->GetTrack()->GetCreatorProcess()->GetProcessName();

  if(step->GetTrack()->GetParentID()>0 
     && 
     step->GetTrack()->GetCreatorProcess()->GetProcessName() == "Decay"
     && 
     particleName=="e+" 
     && 
     prePoint->GetTouchableHandle()->GetVolume()!=fDetector->GetEnvelope() 
     &&
     prePoint->GetTouchableHandle()->GetVolume()!=fDetector->GetWorld() 
     && 
     endPoint->GetTouchableHandle()->GetVolume()==fDetector->GetEnvelope())




//  fRunAction->CountProcesses(procName);
//


//  if (prePoint->GetTouchableHandle()->GetVolume()==fDetector->GetTarget() &&
//      endPoint->GetTouchableHandle()->GetVolume()==fDetector->GetEnvelope())
  {
    G4Track* aTrack = step->GetTrack();
    const G4ParticleDefinition* part = 
      aTrack->GetDynamicParticle()->GetDefinition();
    //    G4cout<<"a "<<particleName<<" left the Box \n";
    G4ThreeVector position  = endPoint->GetPosition();
    G4ThreeVector direction = endPoint->GetMomentumDirection();
    G4double kinEnergy = endPoint->GetKineticEnergy();

    G4ThreeVector beamDirection =
      fPrimary->GetParticleGun()->GetParticleMomentumDirection();
    G4double polZ = endPoint->GetPolarization().z();

    G4double costheta = direction*beamDirection;

    G4double xdir =
      direction*G4PolarizationHelper::GetParticleFrameX(beamDirection);
    G4double ydir =
      direction*G4PolarizationHelper::GetParticleFrameY(beamDirection);

    G4double phi=std::atan2(ydir,xdir);
    
    //G4cout << "phi = " << phi << G4endl;

    auto analysisManager  = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(18, phi);

    //fEvent->AddPhi(phi);
    //fRunAction->FillData(part,kinEnergy,costheta,phi,polZ);
  }
}
