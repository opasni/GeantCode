#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4UnitsTable.hh"
#include "G4TouchableHistory.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(const DetectorConstruction* detectorConstruction,
                      EventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction),
    fProcess()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Get volume of the current step
  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());

  G4VPhysicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

  if (volume == fDetConstruction->GetAbsorberPV()){
  G4String name = step->GetTrack()->GetDefinition()->GetParticleName();
  G4ThreeVector pos = step->GetTrack()->GetPosition();
  G4double timeglob = step->GetTrack()->GetGlobalTime();
  G4double energy = step->GetTrack()->GetKineticEnergy();
  G4String proces = "";//step->GetTrack()->GetCreatorProcess()->GetProcessName();
  if ((name == "gamma")||(name == "e+")||(name == "neutron")||(name == "proton")) {
    fProcess = step->GetTrack()->GetCreatorProcess();
    proces = fProcess->GetProcessName();}  
  if (name == "e-") {
    proces = "eIoni";}
    //proces = fProcess->GetProcessName();}
  //G4int rowNo, columnNo;
  //G4VPhysicalVolume* cellPhysical = touchable->GetVolume(2);
  //rowNo = cellPhysical->GetCopyNo();
  //G4VPhysicalVolume* columnPhysical = touchable->GetVolume(3);
  //columnNo = columnPhysical->GetCopyNo();  
  fEventAction->AddParticle(name, pos[0], pos[1], timeglob, energy, proces);
  step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
