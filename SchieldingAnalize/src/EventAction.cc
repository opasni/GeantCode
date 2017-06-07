#include "EventAction.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>
#include <cmath>
#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
 : G4UserEventAction(),
   fRunAction(runAction),
   fTheta(0), fEnergy(0), fTime(0),fName(),fProcess(),
   fNSensed(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  fTheta.clear();
  fEnergy.clear();
  fTime.clear();
  fName.clear();
  fProcess.clear();
  fNSensed = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Accumulate statistics
  //

  // get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  //std::vector<G4int> parNID;
  for (G4int i=0; i<fNSensed; i++) {
    analysisManager->FillNtupleDColumn(0, fEnergy[i]/MeV);
    analysisManager->FillNtupleDColumn(1, fTheta[i]);
    analysisManager->FillNtupleDColumn(2, fTime[i]);
    analysisManager->FillNtupleSColumn(3, fName[i]);
    analysisManager->FillNtupleSColumn(4, fProcess[i]);
    analysisManager->AddNtupleRow();
  }
  //
  // Print per event (modulo n)
  //
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    //DetectorConstruction::SetTargetLayerThickness(fthicnessD*eventID/printModulo);
    G4cout << "---> End of event: " << eventID << G4endl;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
