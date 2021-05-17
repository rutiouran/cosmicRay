#include "EventAction.hh"
#include "RunAction.hh"

#include "TargetHit.hh"
#include "TargetSD.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include "g4root.hh"
#include <iomanip>

EventAction::EventAction()
: G4UserEventAction(),
  fTargetHCID(-1)
{
  copyNb = DetectorConstruction::GetCopyNb();
} 

EventAction::~EventAction()
{}

TargetHitsCollection*
EventAction::GetTargetHitsCollection(G4int hcID, 
				const G4Event* event) const
{
  auto TargethitsCollection
    = static_cast<TargetHitsCollection*>(
	event->GetHCofThisEvent()->GetHC(hcID));

  if(!TargethitsCollection)
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID" << hcID;
    G4Exception("EventAction::GetHitsCollection()",
	 "MyCode0003", FatalException, msg);
  }

  return TargethitsCollection;
}

void EventAction::BeginOfEventAction(const G4Event*)
{}

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Get hits collection IDs (only once)
  if(fTargetHCID == -1)
  {
     fTargetHCID = G4SDManager::GetSDMpointer()->GetCollectionID("TargetHitsCollection");
  }

  // Get hits collection
  auto targetHC = GetTargetHitsCollection(fTargetHCID, event);
//  if(targetHC == 0) return;

  // Get hit with total values
//  if(targetHC->entries()<=0) return;
  //auto targetHit = (*targetHC)[targetHC->entries()-1];

  //if(targetHit == 0) return;
 
  // Fill histograms, ntuple
  //

  G4double edepd[copyNb] = {0.};
  G4double muTime[copyNb] = {0.}; // The initial time in per scintillant of muon
  G4double neuTime[copyNb] = {0.}; // The initial time in per scintillant of meutrion
  G4double edep = 0; // The total deposit energy

  // Get analysis manager
  auto analysisManager  = G4AnalysisManager::Instance();
 
  for(long unsigned int i=0; i<targetHC->entries(); i++)
  {
     edep+=(*targetHC)[i]->GetEdep();
     edepd[(*targetHC)[i]->GetTargetNb()]+=(*targetHC)[i]->GetEdep();

     if((*targetHC)[i]->GetPid()==-13 && muTime[(*targetHC)[i]->GetTargetNb()]==0)
     {
        muTime[(*targetHC)[i]->GetTargetNb()] = (*targetHC)[i]->GetGlobalTime(); // Get The initial time in per scintillant
     }
     if((*targetHC)[i]->GetPid()==-14 && neuTime[(*targetHC)[i]->GetTargetNb()]==0)
     {
        neuTime[(*targetHC)[i]->GetTargetNb()] = (*targetHC)[i]->GetGlobalTime(); // Get The initial time in per scintillant
     }
  }

  analysisManager->FillNtupleDColumn(0, edep); // The total edep
  
  for(G4int i=0; i<copyNb; i++)
  {
    analysisManager->FillNtupleDColumn(i+1, edepd[i]); // The edep per scintillant
    analysisManager->FillNtupleDColumn(copyNb+i*2+1, muTime[i]); // The time of mu per scintillant
    analysisManager->FillNtupleDColumn(copyNb+(i+1)*2, neuTime[i]); // The time of neutrino per scintillant
  }

  analysisManager->AddNtupleRow();
}
