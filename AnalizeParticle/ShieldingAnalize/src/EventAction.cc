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
  G4int ntupleID;
  int gam=0; int n=0;

  for (G4int i=0; i<fNSensed; i++) {
    if ((fTheta[i]<0.515)&&(0.165<fTheta[i])){
      if (fName[i]=="gamma") {gam++; ntupleID = 0;}
      else if (fName[i]=="neutron") {n++; ntupleID = 1;}

      analysisManager->FillNtupleDColumn(ntupleID, 0, fEnergy[i]/MeV);
      analysisManager->FillNtupleDColumn(ntupleID, 1, fTime[i]);
      analysisManager->FillNtupleDColumn(ntupleID, 2, fTheta[i]);
      analysisManager->FillNtupleSColumn(ntupleID, 3, fProcess[i]);
      analysisManager->AddNtupleRow(ntupleID);
    }
  }
  analysisManager->FillH1(1, gam);
  analysisManager->FillH1(2, n);
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
