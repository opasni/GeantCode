#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
                      const DetectorConstruction* detectorConstruction,
                      EventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  if (volume == fDetConstruction->GetAbsorberPV()) {
      G4int partN = 0;
      G4ThreeVector verpos = step->GetTrack()->GetVertexPosition();
      G4double posZ = verpos.getZ();
      if (posZ>-60){
        if (posZ<-50) partN=1;
        else if (posZ>50) partN=2;
        else if (sqrt(pow(verpos.getX(),2)+pow(verpos.getY(),2))>50) {partN=3;
        G4cout << posZ << G4endl;
      }
        if (partN!=0) {
          // if (partN!=1) G4cout << partN << G4endl;
          G4String name = step->GetTrack()->GetDefinition()->GetParticleName();

          G4ThreeVector pos = step->GetTrack()->GetPosition();
          G4double Theta = pos.getTheta();

          G4String process = step->GetTrack()->GetCreatorProcess()->GetProcessName();

          G4double energy = step->GetTrack()->GetKineticEnergy();

          fEventAction->AddData(Theta, energy, partN, name, process);

        }
      }
      step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
