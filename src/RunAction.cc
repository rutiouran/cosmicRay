#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "g4root.hh"

RunAction::RunAction()
: G4UserRunAction()
{
  // Get target number
  copyNb = DetectorConstruction::GetCopyNb();

  // Set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);

  // Creat analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "using" << analysisManager->GetType() << G4endl;

  // Creat directories
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);

  // Book histograms, ntuple
  // Note: merging ntuples is available only with ROOT output
  //
  G4String title1 = "edep";
  G4String title2 = "Time";

  //ntuple
  analysisManager->CreateNtuple("Edep", "Edep");
  for(G4int i=1; i<=copyNb; i++)
  {
     analysisManager->CreateNtupleDColumn(title1+std::to_string(i));
  }
  analysisManager->CreateNtupleDColumn("TotalEdep");
  analysisManager->FinishNtuple();

  analysisManager->CreateNtuple("DecayTime", "DecayTime");
  for(G4int i=1; i<=copyNb; i++)
  {
  analysisManager->CreateNtupleDColumn(title2+std::to_string(i)+"muon");
  analysisManager->CreateNtupleDColumn(title2+std::to_string(i)+"electron");
  }
  analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  G4String filename = "output";
  analysisManager->OpenFile(filename);
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
