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
   fPhi(0), fTheta(0), fenergy(0), ftime(0), fname(),
   fnSensed(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{
  fPhi.clear();
  fTheta.clear();
  fenergy.clear();
  ftime.clear();
  fname.clear();
  fnSensed = 0;
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

  for (G4int i=0; i<fnSensed; i++) {
    // if (fprocess[i] == "photonNuclear") G4cout << fTheta[i] << G4endl;
    if (fname[i]=="gamma") {gam++; ntupleID = 0;
      // analysisManager->FillH1(3, fTheta[i]);
    }
    else if (fname[i]=="neutron") {n++; ntupleID = 1;
      // analysisManager->FillH1(4, fTheta[i]);
    }

    analysisManager->FillNtupleDColumn(ntupleID, 0, fenergy[i]/MeV);
    analysisManager->FillNtupleDColumn(ntupleID, 1, ftime[i]);
    analysisManager->FillNtupleDColumn(ntupleID, 2, fPhi[i]);
    analysisManager->FillNtupleDColumn(ntupleID, 3, fTheta[i]);
    analysisManager->AddNtupleRow(ntupleID);
  }
  // analysisManager->FillH1(1, gam);
  // analysisManager->FillH1(2, em);
  // analysisManager->FillH1(3, p);
  // analysisManager->FillH1(2, n);
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
