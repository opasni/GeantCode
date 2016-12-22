#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <list>
#include <vector>
#include <algorithm>

class EventAction;
class G4Run;

/// Run action class

class RunAction : public G4UserRunAction
{
  public:
    RunAction(EventAction* eventAction, G4int fnofLayers);
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:

    void ComputeRadiusBin();
    EventAction* fEventAction;

    G4int nofLayers;
    std::list<G4double> fradiusBin;
    std::vector<G4double> radiusBin;
    std::vector<G4int> fnormFactor;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
