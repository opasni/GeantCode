#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction, G4int fnofLayers)
 : G4UserRunAction(),
   fEventAction(eventAction),
    nofLayers(fnofLayers)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);
  analysisManager->SetFileName("Project");

  // Creating histograms
  analysisManager->CreateNtuple("Gamma", "g");
  analysisManager->CreateNtuple("Electron", "e-");
  analysisManager->CreateNtuple("Positron", "e+");
  analysisManager->CreateNtuple("Proton", "p");
  analysisManager->CreateNtuple("Neutron", "n");

  for (int i = 0; i < 5; i++) {
  analysisManager->CreateNtupleDColumn(i, "PositionX");
  analysisManager->CreateNtupleDColumn(i, "PositionY");
  analysisManager->CreateNtupleDColumn(i, "Time");
  analysisManager->CreateNtupleDColumn(i, "Energy");
  analysisManager->CreateNtupleSColumn(i, "Process");
  analysisManager->FinishNtuple(i);
  }

  analysisManager->CreateNtuple("Other", "OtherParticles");
  analysisManager->CreateNtupleSColumn(5, "Name");
  analysisManager->FinishNtuple(5);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......








