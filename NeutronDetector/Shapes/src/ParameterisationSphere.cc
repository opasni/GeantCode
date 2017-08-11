#include "ParameterisationSphere.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"


ChamberParameterisation::ChamberParameterisation(G4int n, std::vector<G4Material*>& mat, G4double radius,
                                                 G4double detXY, G4double tarDist, G4bool XorY)
: G4VNestedParameterisation(),
  fst(n), fMat(mat),
  fRadius(radius),fDetXY(detXY),fTarDist(tarDist),
  fXorY(XorY), fVisAttributes()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ChamberParameterisation::~ChamberParameterisation()
{
  // for (G4int i=0; i<G4int(fVisAttributes.size()); ++i)
  // {
  //   delete fVisAttributes[i];
  // }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Material* ChamberParameterisation::ComputeMaterial(G4VPhysicalVolume *currentVol, const G4int no_lev,
                                                    const G4VTouchable *parentTouchable) {
  if(parentTouchable==0) return fMat[0];
  if (fXorY == false) return fMat[1];
  else {
    // G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    // visAttributes->SetVisibility(false);
    G4LogicalVolume* currentLogVol= currentVol->GetLogicalVolume();
    // G4ThreeVector translX = currentVol->GetTranslation();
    G4double posY = currentVol->GetTranslation().getY();
      // Get the information about the parent volume
    // G4int no_parent_r = parentTouchable->GetReplicaNumber(0);
    G4double posX = (parentTouchable->GetReplicaNumber(0) - 0.5*(fst-1))*(2*fDetXY/fst);
    // G4cout << posX << ' ' <<  posY << ' ' << sqrt(posX*posX + posY*posY) << ' ' << fRadius << G4endl;

    G4Material* material;
    if ((sqrt(posX*posX + posY*posY) < fRadius) || (sqrt(posX*posX + posY*posY) > fDetXY))
    {
      G4VisAttributes* visAttributesf = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
      visAttributesf->SetVisibility(false);
      currentLogVol->SetVisAttributes(visAttributesf);
      fVisAttributes.push_back(visAttributesf);
      material= fMat[1];
    }
    else {
      G4VisAttributes* visAttributest = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
      visAttributest->SetVisibility(true);
      currentLogVol->SetVisAttributes(visAttributest);
      fVisAttributes.push_back(visAttributest);
      material= fMat[0];
    }
    //   // Set the material to the current logical volume
    currentLogVol->SetMaterial( material );
    // delete visAttributes;
    return material;
  }
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
  G4double posX, posY, posZ;
  G4RotationMatrix* rotm  = new G4RotationMatrix();
  if (fXorY == false){
    posX = (1 - fst + 2*copyNo)*fDetXY/(fst);
    posY = 0.,
    posZ = fTarDist - sqrt(fTarDist*fTarDist - posX*posX - posY*posY);
    rotm->rotateY(atan(posX/fTarDist));
  }
  else {
    posX = 0.; // (physVol->GetCopyNo() - 0.5*(fst-1))*(2*fDetXY/fst);
    posY = (1 - fst + 2*copyNo)*fDetXY/(fst);
    posZ = fTarDist - sqrt(fTarDist*fTarDist - posX*posX - posY*posY);
    rotm->rotateX(-atan(posY/fTarDist));
  }
  G4cout << posX << ' ' <<  posY << ' ' << posZ << ' ' << fRadius << G4endl;
  physVol->SetTranslation(G4ThreeVector(posX,posY,posZ));
  physVol->SetRotation(rotm);

}
