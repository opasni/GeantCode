#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>

/// Event action

//class DetectorConstruction;

class EventAction : public G4UserEventAction
{
public:
    EventAction(G4int fnofLayers);
    virtual ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    std::vector<G4double>& GetHadCalEdep() { return fHadCalEdep; }
    
private:
    G4int fHCHCID;
    std::vector<G4double> fHadCalEdep;
    G4int nofLayers;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
