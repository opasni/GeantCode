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

EventAction::EventAction(const DetectorConstruction* detectorConstruction)
: G4UserEventAction(),
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
        HadCalorimeterHit* hit = (*hcHC)[i];
        G4double eDep = hit->GetEdep();
        if (eDep>0.)
        {
          G4int layerNo = hit->GetLayerID();
          G4int rowNo = hit->GetRowID();
          // G4int columnNo = hit->GetColumnID();
          G4int parentNo = hit->GetParentID();
          G4double dtime = hit->GetTime();
          G4ThreeVector pos = hit->GetPos();
          G4ThreeVector posMean = hit->GetPosMean();
          G4double dist = sqrt(pow(pos[0]-posMean[0],2)+pow(pos[1]-posMean[1],2))-posMean[2];
          fPos.push_back(dist);
          G4double theta = atan(sqrt(pos.getX()*pos.getX()+pos.getY()*pos.getY())/(fscintDetails+pos.getZ()));

          // if (dtime > 40) {
          //   analysisManager->FillNtupleDColumn(0, eDep);
          //   analysisManager->FillNtupleDColumn(1, dtime);
          //   analysisManager->FillNtupleIColumn(2, layerNo);
          //   analysisManager->AddNtupleRow();
          // }
          if (dtime > 40) {
            if (dtime < 60) analysisManager->FillH1(1, eDep);
            if (dtime < 85) analysisManager->FillH1(2, eDep);
            if (dtime < 125) analysisManager->FillH1(3, eDep);
          }
          analysisManager->FillH1(7, dist);

          if ((eDep < 20)&&(eDep>16)) analysisManager->FillH1(4, dtime);
          analysisManager->FillH1(6, parentNo);
          analysisManager->FillH2(1, eDep, dtime);
          analysisManager->FillH2(2, eDep, dtime);
          analysisManager->FillH2(3, eDep, dtime);
          analysisManager->FillH2(4, eDep, theta);
          if (dtime > 40) analysisManager->FillH2(5, eDep, theta);

          totalHadHit++;
          totalHadE += eDep;
        }
        // if (p0>0) G4cout << p0<< G4endl;
        fHadCalEdep[i] = totalHadE;
        if (totalHadHit!=0) {
          analysisManager->FillH1(5, totalHadHit);
          // G4double xMean = CalculateMean(0);
          // G4double yMean = CalculateMean(1);
          // G4double xMean = posMean[0];
          // G4double yMean = posMean[1];
          // G4int n = fPos.size();
          // G4cout << "Mean: " << xMean << ' ' << yMean << G4endl;
          // for (int i=0; i<n; i++) G4cout << fPos[i][0] << ' ' << fPos[i][1] << G4endl;
          // G4double dev = CalculateDevXY(xMean, yMean);
          // analysisManager->FillH1(7, dev);
        }
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
