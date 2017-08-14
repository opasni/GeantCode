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
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void AddNumber(G4int num, G4double eDep);

  private:

    G4int fNumber;
    G4double fEDep;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void RunAction::AddNumber(G4int num, G4double eDep) {
  fEDep += eDep;
  fNumber += num;
}

#endif
