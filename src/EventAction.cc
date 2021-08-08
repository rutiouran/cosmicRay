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
//if(targetHC == 0) return;

  // Get hit with total values
//  if(targetHC->entries()<=0) return;
//auto targetHit = (*targetHC)[targetHC->entries()-1];

  //if(targetHit == 0) return;

  // Get analysis manager
  auto analysisManager  = G4AnalysisManager::Instance();
 
  // Fill histograms, ntuple
  //
  G4double TotalEdep = 0.; // The total edep
  for(long unsigned int i=0; i<targetHC->entries(); i++)
  {
    TotalEdep+=(*targetHC)[i]->GetEdep();
  }

  analysisManager->FillNtupleDColumn(0, TotalEdep);
  for(long unsigned int i=0; i<targetHC->entries(); i++)
  {
    analysisManager->FillNtupleDColumn(i+1, (*targetHC)[i]->GetEdep());
    analysisManager->FillNtupleDColumn(2*i+7, (*targetHC)[i]->GetMuTime());
    analysisManager->FillNtupleDColumn(2*i+1+7, (*targetHC)[i]->GetElectronTime());
    analysisManager->FillNtupleDColumn(19, (*targetHC)[i]->GetDecayPosition());
  }
  analysisManager->AddNtupleRow();


//
//  G4double edepd[copyNb] = {0.};
//  G4double muTime[copyNb] = {0.}; // The initial time in per scintillator of muon
//  G4double neuTime[copyNb] = {0.}; // The initial time in per scintillator of meutrion
//  G4double edep = 0; // The total deposit energy
//
//  // Get analysis manager
//  auto analysisManager  = G4AnalysisManager::Instance();
// 
//  for(long unsigned int i=0; i<targetHC->entries(); i++)
//  {
//     edep+=(*targetHC)[i]->GetEdep();
//     edepd[(*targetHC)[i]->GetTargetNb()]+=(*targetHC)[i]->GetEdep();
//
//     if((*targetHC)[i]->GetPid()==-13 && muTime[(*targetHC)[i]->GetTargetNb()]==0)
//     {
//        muTime[(*targetHC)[i]->GetTargetNb()] = (*targetHC)[i]->GetGlobalTime(); // Get The initial time in per scintillator
//     }
//     if((*targetHC)[i]->GetPid()==-14 && neuTime[(*targetHC)[i]->GetTargetNb()]==0)
//     {
//        neuTime[(*targetHC)[i]->GetTargetNb()] = (*targetHC)[i]->GetGlobalTime(); // Get The initial time in per scintillator
//     }
//  }
//
//  analysisManager->FillNtupleDColumn(0, 0, edep); // The total edep
//  for(G4int i=0; i<copyNb; i++)
//  {
//    analysisManager->FillNtupleDColumn(0, i+1, edepd[i]); // The edep per scintillator
//  }
//  analysisManager->AddNtupleRow(0);
//  
//  for(G4int i=0; i<copyNb; i++)
//  {
//    analysisManager->FillNtupleDColumn(1, i*2, muTime[i]); // The time of mu per scintillator
//    analysisManager->FillNtupleDColumn(1, 2*i+1, neuTime[i]); // The time of neutrino per scintillator
//  }
//
//  analysisManager->AddNtupleRow(1);
}
