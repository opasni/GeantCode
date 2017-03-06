#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction(EventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);
  analysisManager->SetFileName("Project");
  //
  // if ( fEventAction ) {
  analysisManager->CreateNtuple("Hits", "Hits");
  analysisManager->CreateNtupleDColumn("Energy"); // column Id = 1
  analysisManager->CreateNtupleDColumn("Time"); // column Id = 2
  analysisManager->CreateNtupleDColumn("VertexPositionZ"); // column Id = 3
  analysisManager->CreateNtupleIColumn("Layer"); // column Id = 4
  analysisManager->FinishNtuple();

  analysisManager->CreateH1("TotalEnergy","Total Energy", 100, 0., 110*MeV);
  analysisManager->CreateH1("Energy","Energy", 100, 0., 110*MeV);
  analysisManager->CreateH1("Time","Time", 100, 0., 200);
  analysisManager->CreateH1("Layer","Layer", 3, 0, 3);

  analysisManager->CreateH2("EnergyLayer","Energy Layer", 100, 0., 110*MeV, 3, 0, 3);
  analysisManager->CreateH2("EnergyTime","Energy Time", 100, 0., 110*MeV, 100, 0, 200);
  analysisManager->CreateH2("TimeLayer","Time Layer", 100, 0., 200, 3, 0, 3);
  analysisManager->CreateH2("TimeLayerCut20","Time Layer (Energy cut at 20MeV)", 100, 0., 200, 3, 0, 3);
  // }
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
