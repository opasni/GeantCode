#include "RunAction.hh"
#include "Analysis.hh"
#include "EventAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


RunAction::RunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(0);
  analysisManager->SetFirstHistoId(1);

  // Creating ntuple
  //
  analysisManager->CreateNtuple("Gamma", "g");
  analysisManager->CreateNtuple("Electron", "e-");
  analysisManager->CreateNtuple("Proton", "p");
  analysisManager->CreateNtuple("Neutron", "n");

  for (int i = 0; i < 4; i++) {
  analysisManager->CreateNtupleDColumn(i,"KE");
  analysisManager->CreateNtupleDColumn(i,"Time");
  analysisManager->CreateNtupleDColumn(i,"Phi");
  analysisManager->CreateNtupleDColumn(i,"Theta");
  analysisManager->CreateNtupleIColumn(i,"ParentID");
  analysisManager->CreateNtupleSColumn(i,"Name");
  analysisManager->FinishNtuple(i);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output files
  //
  G4String fileNameRoot = "Project";
  analysisManager->OpenFile(fileNameRoot);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//class EventAction;

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  //EventAction ev;
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
