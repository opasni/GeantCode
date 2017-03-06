#include "Parameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"


ChamberParameterisation::ChamberParameterisation(G4int n, std::vector<G4Material*>& mat)
: G4VNestedParameterisation(),fst(n),fMat(mat)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChamberParameterisation::~ChamberParameterisation()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ChamberParameterisation::ComputeMaterial(G4VPhysicalVolume *currentVol, const G4int no_lev, 
                                                    const G4VTouchable *parentTouchable) {
  if(parentTouchable==0) return fMat[0];
  
  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  //visAttributes->SetVisibility(false);
  G4LogicalVolume* currentLogVol= currentVol->GetLogicalVolume();   
  
    // Get the information about the parent volume  
  G4int no_parent_r = parentTouchable->GetReplicaNumber(0);
  G4int no_parent_c = parentTouchable->GetReplicaNumber(1);
  
  G4Material* material=0;  
  if (((no_parent_r==8) ||(no_parent_r==9) ||(no_parent_r==10) ||(no_parent_r==11))
            && ((no_parent_c==8) ||(no_parent_c==9) ||(no_parent_c==10) ||(no_parent_c==11))) 
  {
    visAttributes->SetVisibility(false);
    currentLogVol->SetVisAttributes(visAttributes);
    material= fMat[1];
  }
  else {
    visAttributes->SetVisibility(true);
    currentLogVol->SetVisAttributes(visAttributes);
    material= fMat[0];
  }
    // Set the material to the current logical volume 
  currentLogVol->SetMaterial( material ); 
  delete visAttributes;
  return material;
}

 G4int ChamberParameterisation::GetNumberOfMaterials() const{
  return fMat.size();
}

G4Material* ChamberParameterisation::GetMaterial(G4int i) const{
  return fMat[i];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ChamberParameterisation::ComputeTransformation (const G4int copyNo,G4VPhysicalVolume *physVol) const 
{
    G4double Zposition = (1 - fst + 2*copyNo)*50*cm/(2*fst);
    physVol->SetTranslation(G4ThreeVector(0.,0.,Zposition));
}
