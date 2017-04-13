#ifndef EventAction_h
#define EventAction_h 1


#include "G4UserEventAction.hh"
#include "DetectorConstruction.hh"
#include "globals.hh"

#include <vector>

/// Event action

//class DetectorConstruction;

class EventAction : public G4UserEventAction
{
public:
    EventAction(const DetectorConstruction* detectorConstruction);
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    std::vector<G4double>& GetHadCalEdep() { return fHadCalEdep; }

private:
    G4int fHCHCID;
    std::vector<G4double> fHadCalEdep;
    std::vector<G4ThreeVector> fPos;
    G4double fscintDetails;
    G4int nofLayers;
    G4int nofLayersZ;
    G4double CalculateMean(G4int);
    G4double CalculateDevXY(G4double, G4double);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double EventAction::CalculateMean(G4int xyz)
{
  G4int n = fPos.size();
  G4double mean = 0.;
  for (int i=0; i<n; i++) mean+=fPos[i][xyz];
  return (mean/n);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double EventAction::CalculateDevXY(G4double xMean, G4double yMean)
{
  G4int n = fPos.size();
  G4double dev = 0.;
  for (int i=0; i<n; i++) dev+=(pow(fPos[i][0]-xMean,2)+pow(fPos[i][1]-yMean,2));
  return sqrt(dev/n);
}


#endif
