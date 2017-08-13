#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "DetectorConstruction.hh"
#include "globals.hh"

#include <vector>

/// Event action
class RunAction;

class EventAction : public G4UserEventAction
{
public:
    EventAction(RunAction* runAction, const DetectorConstruction* detectorConstruction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    std::vector<G4double>& GetHadCalEdep() { return fHadCalEdep; }

private:
    RunAction* fRunAction;
    G4int fHCHCID;
    std::vector<G4double> fHadCalEdep;
    std::vector<G4double> fPos;
    G4double fscintDetails;
    G4int nofLayers;
    G4int nofLayersZ;
};


#endif
