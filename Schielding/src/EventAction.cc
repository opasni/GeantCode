#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"
#include <iomanip>
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(G4int fnofLayers)
 : G4UserEventAction(), nofLayers(fnofLayers), fname(), fposX(), fposY(), ftime(), fenergy(), fprocess()
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{  
  fname.clear();
  fposX.clear();
  fposY.clear();
  ftime.clear();
  fenergy.clear();
  fprocess.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  // Accumulate statistics
  //

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  G4int entries = fname.size();
  int gam=0; int em=0; int ep=0; int p=0; int n=0; int ost=0;
  G4int ntupleID;

  if (entries != 0){
    for (G4int i=0; i<entries; i++)
    {
      if (fname[i]=="gamma") {gam++; ntupleID = 0;}
      //if (fname[i]=="gamma") {gam++; continue;}
      else if (fname[i]=="e-") {em++; ntupleID = 1;}
      else if (fname[i]=="e+") {ep++; ntupleID = 2;}
      else if (fname[i]=="proton") {p++; ntupleID = 3;}
      else if (fname[i]=="neutron") {n++; ntupleID = 4;}
      else {
        ost++; 
        analysisManager->FillNtupleSColumn(5,0, fname[i]);
        analysisManager->AddNtupleRow(5);
        continue;
      }
      analysisManager->FillNtupleDColumn(ntupleID,0, fposX[i]);
      analysisManager->FillNtupleDColumn(ntupleID,1, fposY[i]);
      analysisManager->FillNtupleDColumn(ntupleID,2, ftime[i]);
      analysisManager->FillNtupleDColumn(ntupleID,3, fenergy[i]);
      analysisManager->FillNtupleSColumn(ntupleID,4, fprocess[i]);
      analysisManager->AddNtupleRow(ntupleID);
  }
}


  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     
    G4cout
       << "   Photons detected: " << std::setw(7)
                                         << gam
       << G4endl;
    G4cout
       << "   Electrons detected: " << std::setw(7)
                                         << em
       << G4endl;   
    G4cout
       << "   Positrons detected: " << std::setw(7)
                                         << ep
       << G4endl;
    G4cout
       << "   Protons detected: " << std::setw(7)
                                         << p
       << G4endl;
    G4cout
       << "   Neutrons detected: " << std::setw(7)
                                         << n
       << G4endl;
    G4cout
       << "   Others detected: " << std::setw(7)
                                         << ost
       << G4endl;
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


