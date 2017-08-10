
#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(const DetectorConstruction* detectorConstruction,
                    EventAction* eventAction);
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
    
private:
  const DetectorConstruction* fDetConstruction;
  EventAction*  fEventAction;  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
