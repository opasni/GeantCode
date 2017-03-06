
 #ifndef Parameterisation_H
 #define Parameterisation_H 1

 #include "globals.hh"
 #include "G4VPVParameterisation.hh"
 #include "G4VNestedParameterisation.hh"
 #include "G4Material.hh"
 #include "G4Types.hh"
 #include "G4ThreeVector.hh"
 #include "G4VTouchable.hh"
 #include <vector>

 class G4VPhysicalVolume;
 class G4VTouchable; 
 class G4VSolid;
 class G4Material;

 class ChamberParameterisation : public G4VNestedParameterisation
 {
  public:
    ChamberParameterisation(G4int n, std::vector<G4Material*>& mat);
    virtual ~ChamberParameterisation();

    virtual void ComputeTransformation (const G4int copyNo, G4VPhysicalVolume* physVol) const; // position, rotation
    
    // virtual void ComputeDimensions (G4Box& trackerLayer, const G4int copyNo, const G4VPhysicalVolume* physVol) const; // size

    virtual G4Material* ComputeMaterial(G4VPhysicalVolume *currentVol, const G4int no_lev, const G4VTouchable *parentTouch);
    virtual G4int GetNumberOfMaterials() const;
    virtual G4Material* GetMaterial(G4int idx) const;

    using G4VNestedParameterisation::ComputeMaterial;

  private:
  	G4int fst;
    std::vector<G4Material*> fMat;
// G4VTouchable should not be used for ordinary parameterization*/
 };



 #endif