#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::ActionInitialization
                            (DetectorConstruction* detConstruction)
 : G4VUserActionInitialization(),
   fDetConstruction(detConstruction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ActionInitialization::~ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::BuildForMaster() const
{
  EventAction* eventAction = 0;
  SetUserAction(new RunAction(eventAction, fDetConstruction->GetNumberOfLayers()));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);

  EventAction* eventAction = new EventAction(fDetConstruction->GetNumberOfLayers());
  SetUserAction(eventAction);
  SetUserAction(new RunAction(eventAction, fDetConstruction->GetNumberOfLayers()));

  SetUserAction(new SteppingAction(fDetConstruction,eventAction));
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......