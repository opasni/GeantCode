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
  analysisManager->CreateNtuple("Hits", "Hits");
  analysisManager->CreateNtupleDColumn("Energy"); // column Id = 1
  analysisManager->CreateNtupleDColumn("Time"); // column Id = 2
  analysisManager->CreateNtupleDColumn("Width"); // column Id = 4
  analysisManager->CreateNtupleDColumn("Depth"); // column Id = 4
  analysisManager->CreateNtupleDColumn("Theta"); // column Id = 4
  analysisManager->CreateNtupleIColumn("Hit"); // column Id = 4
  analysisManager->FinishNtuple();

  analysisManager->CreateH1("Energy","", 100, 0., 110*MeV);
  analysisManager->CreateH1("Time","", 60, 0., 125*ns);
  analysisManager->CreateH1("Theta","", 30, 0.165, 0.515);
  analysisManager->CreateH1("Width","", 80, 10, 400);
  analysisManager->CreateH1("Depth","", 80, 0, 500);
  analysisManager->CreateH1("Layer","", 3, 0, 3);
  analysisManager->CreateH1("WidthAv","", 80, 10, 400);
  analysisManager->CreateH1("DepthAV","", 80, 0, 500);
  analysisManager->CreateH1("EnergyAv","", 100, 0., 110*MeV);
  analysisManager->CreateH1("EnergyTot","", 100, 0., 110*MeV);

  analysisManager->CreateH2("EnergyTime","Depth of signal", 100, 0., 110*MeV, 60, 0., 125*ns);
  analysisManager->CreateH2("EnergyTheta","Depth of signal", 100, 0., 110*MeV, 30, 0.165, 0.515);
  analysisManager->CreateH2("TimeTheta","Depth of signal", 60, 0., 125*ns, 30, 0.165, 0.515);
  analysisManager->CreateH2("TimeWidth","Depth of signal", 60, 0., 125*ns, 80, 10, 400);
  analysisManager->CreateH2("TimeDepth","Depth of signal", 60, 0., 125*ns, 80, 0, 500);

  analysisManager->CreateH2("EnergyWidth","Width of signal", 100, 0., 110*MeV, 80, 10, 400);
  analysisManager->CreateH2("EnergyDepth","Depth of signal", 100, 0., 110*MeV, 80, 0, 500);
  analysisManager->CreateH2("WidthDepth","Depth of signal", 80, 0, 500, 80, 10, 400);

  analysisManager->CreateH2("EnergyWidth2","Width of signal", 100, 0., 110*MeV, 80, 10, 400);
  analysisManager->CreateH2("EnergyDepth2","Depth of signal", 100, 0., 110*MeV, 80, 0, 500);

  analysisManager->CreateH2("WidthDepth2","Depth of signal", 80, 0, 500, 80, 10, 400);
  analysisManager->CreateH2("WidthDepth3","Depth of signal", 80, 0, 500, 80, 10, 400);


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
