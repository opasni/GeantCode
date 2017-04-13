#ifndef HadCalorimeterSD_h
#define HadCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"

#include "HadCalorimeterHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

/// Hadron calorimeter sensitive detector

class HadCalorimeterSD : public G4VSensitiveDetector
{
public:
    HadCalorimeterSD(G4String name, G4int fnofLayers, G4int fnofLayersZ);
    virtual ~HadCalorimeterSD();

    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);

private:
    HadCalorimeterHitsCollection* fHitsCollection;
    G4int fHCID;
    G4int nofLayers;
    G4int nofLayersZ;
    G4int fFirst=0;
    G4double xMean;
    G4double yMean;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
