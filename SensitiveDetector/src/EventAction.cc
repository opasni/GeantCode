#include "EventAction.hh"
#include "HadCalorimeterHit.hh"
#include "Analysis.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "G4UnitsTable.hh"

#include <ios>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(G4int fnofLayers)
: G4UserEventAction(),
    fHCHCID(-1),
    fHadCalEdep(),
    nofLayers(fnofLayers)
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  fHadCalEdep.resize(nofLayers*nofLayers, 0.);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
    if (fHCHCID==-1) {
      G4SDManager* sdManager = G4SDManager::GetSDMpointer();
      fHCHCID = sdManager->GetCollectionID("HadCalorimeter/HadCalorimeterColl");
    }
}     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce) return;   

    // Get hits collections      
    HadCalorimeterHitsCollection* hcHC 
      = static_cast<HadCalorimeterHitsCollection*>(hce->GetHC(fHCHCID));
      
    if (!hcHC) return;   
    
    //
    // Fill histograms & ntuple
    // 
    
    // Get analysis manager
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();   
    //G4int nmid = (nofLayers/2);

    // HCEnergy
    G4int totalHadHit = 0;
    G4double totalHadE = 0.;
    for (G4int i=0;i<nofLayers*nofLayers;i++)
    {
        HadCalorimeterHit* hit = (*hcHC)[i];
        G4double eDep = hit->GetEdep();
        G4double dtime = hit->GetTime();
        if (eDep>0.)
        {
            G4int layerID = hit->GetRowID();
            G4int columnID = hit->GetColumnID();
            //G4cout << layerID << G4endl;
            G4ThreeVector pos = hit->GetPos();
            //if ((pos[0]<100) && (pos[0]>=-100) && (pos[1]<100) && (pos[1]>=-100)) G4int a;
            //if ((posX<100) && (posX>=-100) && (posY<100) && (posY>=-100)) G4int a;
            //else    {
            analysisManager->FillNtupleDColumn(1, layerID);
            analysisManager->FillNtupleDColumn(2, columnID);
            analysisManager->FillNtupleDColumn(3, dtime);
            analysisManager->FillNtupleDColumn(4, pos[0]);
            analysisManager->FillNtupleDColumn(5, pos[1]);
            totalHadHit++;
            totalHadE += eDep;
            //}
        }
        fHadCalEdep[i] = eDep;
    }
    if (totalHadE != 0.){
        analysisManager->FillNtupleDColumn(0, totalHadE/MeV);
        analysisManager->AddNtupleRow();  
    }
    //
    // Print diagnostics
    // 
    
    G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( printModulo==0 || event->GetEventID() % printModulo != 0) return;
    
    G4PrimaryParticle* primary = event->GetPrimaryVertex(0)->GetPrimary(0);
    G4cout << G4endl
           << ">>> Event " << event->GetEventID() << " >>> Simulation truth : "
           << primary->GetG4code()->GetParticleName()
           << " " << primary->GetMomentum() << G4endl;

    // Had calorimeter
    G4cout << "Hadron Calorimeter has " << totalHadHit << " hits. Total Edep is "
    << totalHadE/MeV << " (MeV)" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......