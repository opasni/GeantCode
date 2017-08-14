#include "RunAction.hh"
#include "EventAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
 : G4UserRunAction(),
   fNumber(0),
   fEDep(0)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);
  analysisManager->SetFileName("Project");
  //

  analysisManager->CreateH1("Energy","", 100, 0., 110*MeV);
  analysisManager->CreateH1("Time","", 60, 0., 125*ns);
  analysisManager->CreateH1("Theta","", 30, 0.165, 0.515);
  analysisManager->CreateH1("Width","", 80, 10, 400);
  analysisManager->CreateH1("Depth","", 80, 0, 500);

  analysisManager->CreateH2("EnergyTime","Depth of signal", 100, 0., 110*MeV, 60, 0., 125*ns);
  analysisManager->CreateH2("EnergyTheta","Depth of signal", 100, 0., 110*MeV, 30, 0.165, 0.515);
  analysisManager->CreateH2("TimeTheta","Depth of signal", 60, 0., 125*ns, 30, 0.165, 0.515);
  analysisManager->CreateH2("TimeWidth","Depth of signal", 60, 0., 125*ns, 80, 10, 400);
  analysisManager->CreateH2("TimeDepth","Depth of signal", 60, 0., 125*ns, 80, 0, 500);

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
  fNumber = 0;
  fEDep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << fNumber <<  '\t' << fEDep << G4endl;
  std::ofstream myfile;
  myfile.open ("data", std::ios::out | std::ios::app );
  myfile << fNumber << '\t' << fEDep << G4endl;
  myfile.close();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
