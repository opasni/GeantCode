#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction, G4String particle)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction),
   fParticleDef(particle)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  EventAction* eventAction = 0;
  SetUserAction(new RunAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  RunAction* runAction = new RunAction;
  EventAction* eventAction = new EventAction(runAction, fDetConstruction);
  PrimaryGeneratorAction* primaryAction = new PrimaryGeneratorAction(fParticleDef);
  SetUserAction(eventAction);
  SetUserAction(primaryAction);
  SetUserAction(runAction);

  // SetUserAction(new SteppingAction(fDetConstruction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
