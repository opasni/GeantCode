#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"

#include <vector>

class MagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger; 

/// Detector construction

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
    void ConstructTarget(G4double layer_thickness,G4double outRlay1,
                        G4double hz,G4double startAngle,G4double spanningAngle,
                        G4double target_posz, G4Material* vacuum, G4Material* aluminium,
                        G4Material* lH2, G4LogicalVolume* worldLogical);

    void ConstructMagnet(G4double magZR, G4double magsinR, G4double magcosR,
                        G4Material* vacuum, G4LogicalVolume* worldLogical);

    void ConstructScintillator(G4double detectXY, G4double detectZ, G4double detsinR, 
                        G4double detcosR, G4int nofLayersZ, G4double rotTheta,
                        std::vector<G4Material*> detectMat, G4LogicalVolume* worldLogical);    

    void ConstructMaterials();

    G4int GetNumberOfLayers() const;
    
    const G4VPhysicalVolume* GetAbsorberPV() const;

private:

    G4bool fcheckOverlaps;
    G4GenericMessenger* fMessenger;
    
    static G4ThreadLocal MagneticField* fMagneticField;
    static G4ThreadLocal G4FieldManager* fFieldMgr;
    
    G4VPhysicalVolume* fHadCalScintiPV;
    G4LogicalVolume* fscintScintLogical;
    G4LogicalVolume* fMagneticLogical;

    std::vector<G4VisAttributes*> fVisAttributes;
    
    G4int fnofLayers;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4int DetectorConstruction::GetNumberOfLayers() const { 
    return fnofLayers; 
}

inline const G4VPhysicalVolume* DetectorConstruction::GetAbsorberPV() const { 
  return fHadCalScintiPV; 
}

#endif
