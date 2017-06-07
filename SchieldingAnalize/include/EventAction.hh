#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4DataVector.hh"
#include <vector>

/// Event action class
///
class RunAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);

    void AddData(G4double theta, G4double energy,
                 G4double time, G4String name, G4String process);


  private:
    RunAction* fRunAction;
    std::vector<G4double> fTheta;
    std::vector<G4double> fEnergy;
    std::vector<G4double> fTime;
    std::vector<G4String> fName;
    std::vector<G4String> fProcess;
    G4int fNSensed;
};

// inline functions

inline void EventAction::AddData(G4double theta, G4double energy,
                                 G4double time, G4String name, G4String process) {
  fTheta.push_back(theta);
  fEnergy.push_back(energy);
  fTime.push_back(time);
  fName.push_back(name);
  fProcess.push_back(process);
  fNSensed++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
