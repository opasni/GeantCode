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
    
    void AddData(G4double phi, G4double theta, G4double energy, 
                 G4double timeglob, G4int parID, G4String name, G4String process);
 
    
  private:
    RunAction* fRunAction;
    std::vector<G4double> fPhi;
    std::vector<G4double> fTheta;
    std::vector<G4double> fenergy;
    std::vector<G4double> ftime;
    std::vector<G4int> fparID;
    std::vector<G4String> fname;
    std::vector<G4String> fprocess;
    G4int fnSensed;
};

// inline functions

inline void EventAction::AddData(G4double phi, G4double theta, G4double energy, 
                                 G4double timeglob, G4int parID, G4String name, G4String process) {
  fPhi.push_back(phi);
  fTheta.push_back(theta);
  fenergy.push_back(energy);
  ftime.push_back(timeglob);
  fparID.push_back(parID);
  fname.push_back(name);
  fprocess.push_back(process);
  fnSensed++;
}
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
