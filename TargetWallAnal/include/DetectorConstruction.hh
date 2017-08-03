
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4Tubs;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(G4String tarmat);
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    //virtual void ConstructSDandField();

    // get methods
    //
    const G4VPhysicalVolume* GetAbsorberPV() const;
    const G4Tubs* GetTargetPV() const;

  private:
    // methods
    //
    //void SetTargetLayerThickness(G4double newlayerticness);
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    // data members
    //                                      // magnetic field messenger
    G4VPhysicalVolume*   fAbsorberPV; // the absorber physical volume
    G4Tubs*   fTargetTub;
    G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
    G4String ftarmat;
};


// inline functions

inline const G4VPhysicalVolume* DetectorConstruction::GetAbsorberPV() const {
  return fAbsorberPV;
}

inline const G4Tubs* DetectorConstruction::GetTargetPV() const {
  return fTargetTub;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
