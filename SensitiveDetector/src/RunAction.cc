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
  //analysisManager->SetFirstHistoId(1);
  analysisManager->SetFileName("Project");
  //
  //if ( fEventAction ) {
    analysisManager->CreateNtuple("1", "Hits");
    analysisManager->CreateNtupleDColumn("Energy"); // column Id = 1
    analysisManager->CreateNtupleDColumn("LayerID"); // column Id = 1
    analysisManager->CreateNtupleDColumn("ColumnID"); // column Id = 1
    analysisManager->CreateNtupleDColumn("Time"); // column Id = 1
    analysisManager->CreateNtupleDColumn("PositionX"); // column Id = 1
    analysisManager->CreateNtupleDColumn("PositionY"); // column Id = 1
    analysisManager->FinishNtuple();
  //}
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

