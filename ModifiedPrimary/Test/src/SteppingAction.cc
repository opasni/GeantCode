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
  G4int parentID = 0;
  G4VPhysicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  if (volume == fDetConstruction->GetAbsorberPV()) {
      G4String name = step->GetTrack()->GetDefinition()->GetParticleName();
      if (( name == "neutron") || (name == "gamma")) {
        G4ThreeVector pos = step->GetTrack()->GetPosition();
        G4double energy = step->GetTrack()->GetKineticEnergy();
        G4double timeglob = step->GetTrack()->GetGlobalTime();
        G4double Phi = pos.getPhi();
        G4double Theta = pos.getTheta();
        fEventAction->AddData(Phi,Theta, energy, timeglob, name);
      }
      step->GetTrack()->SetTrackStatus(fStopAndKill);

  } //else if ( name == "proton" && volume == fDetConstruction->GetTargetPV() ) {
    //fEventAction->AddProto();
  //}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
