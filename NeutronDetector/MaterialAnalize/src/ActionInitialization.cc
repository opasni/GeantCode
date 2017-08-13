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
  SetUserAction(new RunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  EventAction* eventAction = new EventAction(fDetConstruction);
  SetUserAction(eventAction);

  PrimaryGeneratorAction* primaryAction = new PrimaryGeneratorAction(fParticleDef);
  SetUserAction(primaryAction);

  SetUserAction(new RunAction(eventAction));

  // SetUserAction(new SteppingAction(fDetConstruction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
