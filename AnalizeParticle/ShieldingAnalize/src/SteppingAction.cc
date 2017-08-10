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

// get volume of the current step
  G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  if (volume == fDetConstruction->GetKillerPV()) {
      G4String name = step->GetTrack()->GetDefinition()->GetParticleName();
      if ((name == "proton")||(name == "e-")) step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
  else if (volume == fDetConstruction->GetAbsorberPV()) {
      G4double Theta = step->GetTrack()->GetPosition().getTheta();
      // G4cout << Theta << G4endl;
      // if ((Theta<0.515)&&(Theta>0.165)) {
        // G4cout << step->GetTrack()->GetMomentumDirection() << G4endl;
        G4String name = step->GetTrack()->GetDefinition()->GetParticleName();

        G4double timeglob = step->GetTrack()->GetGlobalTime();

        G4String process = step->GetTrack()->GetCreatorProcess()->GetProcessName();
        // G4String process = " ";

        G4double energy = step->GetTrack()->GetKineticEnergy();
        if ((name == "gamma")||(name == "neutron")) fEventAction->AddData(Theta, energy, timeglob, name, process);
      // }
      step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
