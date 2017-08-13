#include "EventAction.hh"
#include "RunAction.hh"
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

EventAction::EventAction(RunAction* runAction, const DetectorConstruction* detectorConstruction)
: G4UserEventAction(),
    fRunAction(runAction),
    fHCHCID(-1),
    fHadCalEdep(),
    fscintDetails(detectorConstruction->GetScintPosition()),
    nofLayers(detectorConstruction->GetNumberOfLayers()),
    nofLayersZ(detectorConstruction->GetNumberOfLayersZ())
{
  G4cout << nofLayers << ' ' << nofLayersZ << G4endl;
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  fHadCalEdep.resize(nofLayers*nofLayers*nofLayersZ, 0.);
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

    // HCEnergy
    G4int totalHadHit = 0;
    G4double totalHadE = 0.;

    for (G4int i=0;i<nofLayers*nofLayers*nofLayersZ;i++)
    {
        // G4cout << i << G4endl;
        HadCalorimeterHit* hit = (*hcHC)[i];
        G4double eDep = hit->GetEdep();
        if (eDep>0.)
        {
          analysisManager->FillH1(1, eDep);
          totalHadHit++;
          totalHadE += eDep;
          fRunAction->AddNumber(1);
        }
        // if (p0>0) G4cout << p0<< G4endl;
        fHadCalEdep[i] = totalHadE;
        // if (totalHadHit!=0) {
          // analysisManager->FillH1(2, totalHadE);
        // }
    }

    //
    // Print diagnostics
    //

    // G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    // if ( printModulo==0 || event->GetEventID() % printModulo != 0) return;
    //
    // G4PrimaryParticle* primary = event->GetPrimaryVertex(0)->GetPrimary(0);
    // G4cout << G4endl
    //        << ">>> Event " << event->GetEventID() << " >>> Simulation truth : "
    //        << primary->GetG4code()->GetParticleName()
    //        << " " << primary->GetMomentum() << G4endl;
    //
    // // Had calorimeter
    // G4cout << "Hadron Calorimeter has " << totalHadHit << " hits. Total Edep is "
    // << totalHadE/MeV << " (MeV)" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
