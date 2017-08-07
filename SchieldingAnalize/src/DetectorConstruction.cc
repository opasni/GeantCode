#include "DetectorConstruction.hh"
//#include "Parameterisation.hh"
//#include "ParameterisationPHI.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UniformElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4PVParameterised.hh"
#include "G4VPVParameterisation.hh"
#include "G4VNestedParameterisation.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4SubtractionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(G4double shield)
 : G4VUserDetectorConstruction(),
   fAbsorberPV(0),
   fKillerPV(0),
   fCheckOverlaps(true),
   fShieldThick(shield)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Define materials
  DefineMaterials();

  // Define volumes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // Lead material defined using NIST Manager
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_lH2");
  nistManager->FindOrBuildMaterial("G4_Be");
  nistManager->FindOrBuildMaterial("G4_Pb");
  G4double z, a, density;
  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole, density= universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  // G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

  G4Material* defaultMaterial = G4Material::GetMaterial("Galactic");
  G4Material* lH2 = G4Material::GetMaterial("G4_lH2");
  // G4Material* lH2 = G4Material::GetMaterial("G4_lH2");
  G4Material* lead = G4Material::GetMaterial("G4_Pb");
  G4Material* tarwall_mat;
  G4Material* tarwind_mat;
  // if (ftarmat == "yes") {
  tarwall_mat = G4Material::GetMaterial("G4_Al");
  tarwind_mat = G4Material::GetMaterial("G4_Be");
  // }
  // else {
  //   tarwall_mat = G4Material::GetMaterial("Galactic");
  //   tarwind_mat = G4Material::GetMaterial("Galactic");
  // }
  //G4Material* scint_mat = G4Material::GetMaterial("G4_POLYETHYLENE");
  G4Material* scint_mat = G4Material::GetMaterial("Galactic");

  //
  // World
  //
  //G4double env_sizeXY = 120*cm, env_sizeZ = 180*cm;
  G4double world_sizeXY = 1.2*1100*cm;
  G4double world_sizeZ  = 1600*cm;
  G4Box* solidWorld = new G4Box("World", 0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, defaultMaterial, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, fCheckOverlaps);

  //
  // Mother Target
  //
  G4double wall_thickness = 100*um;
  G4double window_thickness = 10*um;

  G4double outRlay1 = 2.5*cm;
  G4double hz = 20*cm;
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;
  //G4double target_posz = 0.5*(-world_sizeZ+1*hz)+1*cm;

  G4ThreeVector tarpos = G4ThreeVector(0, 0, 0);
  G4Tubs* solidtar = new G4Tubs("Target", 0.*cm, outRlay1+wall_thickness, 0.5*hz+window_thickness, startAngle, spanningAngle);
  G4LogicalVolume* logictar = new G4LogicalVolume(solidtar, defaultMaterial, "Target");
  new G4PVPlacement(0, tarpos, logictar, "Target", logicWorld, false, 0, fCheckOverlaps);

  //
  // Target Layer
  //
  G4ThreeVector laypos1 = G4ThreeVector(0, 0, 0);
  G4ThreeVector laypos2 = G4ThreeVector(0, 0, 0.5*(hz+window_thickness));
  G4ThreeVector laypos3 = G4ThreeVector(0, 0, -0.5*(hz+window_thickness));

  G4Tubs* solidtarlayer1
    = new G4Tubs("Target Wall", outRlay1, outRlay1 + wall_thickness, 0.5*hz, startAngle, spanningAngle);
  G4Tubs* solidtarlayer2
    = new G4Tubs("Target Wind 1", 0.*cm, outRlay1+wall_thickness, (0.5*window_thickness), startAngle, spanningAngle);
  G4Tubs* solidtarlayer3
    = new G4Tubs("Target Wind 2", 0.*cm, outRlay1+wall_thickness, (0.5*window_thickness), startAngle, spanningAngle);

  G4LogicalVolume* logictarlayer1 = new G4LogicalVolume(solidtarlayer1, tarwall_mat, "Target Wall");
  G4LogicalVolume* logictarlayer2 = new G4LogicalVolume(solidtarlayer2, tarwind_mat, "Target Wind 1");
  G4LogicalVolume* logictarlayer3 = new G4LogicalVolume(solidtarlayer3, tarwind_mat, "Target Wind 2");

  new G4PVPlacement(0, laypos1, logictarlayer1, "Target Wall", logictar, false, 0, fCheckOverlaps);
  new G4PVPlacement(0, laypos2, logictarlayer2, "Target Wind 1", logictar, false, 0, fCheckOverlaps);
  new G4PVPlacement(0, laypos3, logictarlayer3, "Target Wind 2", logictar, false, 0, fCheckOverlaps);

  //
  // Target True

  G4ThreeVector pos1a = G4ThreeVector(0, 0, 0);
  G4Tubs* solidtarget = new G4Tubs("Target Center", 0.*cm, outRlay1, 0.5*hz, startAngle, spanningAngle);
  G4LogicalVolume* logictarget = new G4LogicalVolume(solidtarget, lH2, "Target Center");
  fTargetPV = new G4PVPlacement(0, pos1a, logictarget, "Target Center", logictar, false, 0, fCheckOverlaps);

  //
  // Scintillator
  //
  // Sphere

  G4ThreeVector scint_pos = G4ThreeVector(0, 0, 0);

  G4double scint_Rmin = 375*cm;
  G4double scint_Rmax = 395*cm;
  G4double scint_PhiS = 0;
  G4double scint_PhiD = 2*(pi);
  G4double scint_ThetaS = 0;
  G4double scint_ThetaD = pi;

  G4Sphere* solidscint = new G4Sphere("Scintillator Mother", scint_Rmin, scint_Rmax, scint_PhiS, scint_PhiD, scint_ThetaS, scint_ThetaD);
  G4LogicalVolume* logicscint = new G4LogicalVolume(solidscint, scint_mat, "Scintillator Mother");
  fAbsorberPV = new G4PVPlacement(0, scint_pos, logicscint, "Scintillator Mother", logicWorld, false, 0, fCheckOverlaps);

  G4Sphere* solidscintP = new G4Sphere("Scintillator Part", scint_Rmin, scint_Rmax, scint_PhiS, scint_PhiD, scint_ThetaS, 0.5*scint_ThetaD);
  G4LogicalVolume* logicscintP = new G4LogicalVolume(solidscintP, scint_mat, "Scintillator Part");
  new G4PVPlacement(0, scint_pos, logicscintP, "Scintillator PArt", logicWorld, false, 0, false);


  //
  // Killer electron
  //

  G4ThreeVector killer_pos = G4ThreeVector(0, 0, 0);

  G4double killer_Rmin = 20*cm;
  G4double killer_Rmax = 30*cm;
  G4double killer_PhiS = 0;
  G4double killer_PhiD = 2*(pi);
  G4double killer_ThetaS = 0;
  G4double killer_ThetaD = pi;

  G4Sphere* solidkiller = new G4Sphere("Killer Mother", killer_Rmin, killer_Rmax, killer_PhiS, killer_PhiD, killer_ThetaS, killer_ThetaD);
  G4LogicalVolume* logickiller = new G4LogicalVolume(solidkiller, scint_mat, "Killer Mother");
  fKillerPV = new G4PVPlacement(0, killer_pos, logickiller, "Killer Mother", logicWorld,
                                false, 0, fCheckOverlaps);

  //
  // Schield
  //


  G4double scheld_Z = 0.75*m;
  G4double scheld_thick = fShieldThick*cm;

  G4ThreeVector scheld_pos = G4ThreeVector(0, 0, scheld_Z+0.5*scheld_thick);

  G4double schieldINXY = tan(0.165)*(scheld_Z+scheld_thick);
  // G4double schieldINXY = 0;
  G4double schieldXY = tan(0.515)*scheld_Z/sqrt(2);

  // G4VSolid* boxS1 = new G4Box("BoxS #1",schieldXY,schieldXY,0.5*scheld_thick);
  // G4VSolid* boxS2 = new G4Box("BoxS #2",schieldINXY,schieldINXY,0.5*scheld_thick);
  //
  // G4VSolid* schieldSolid = new G4SubtractionSolid("Schield", boxS1, boxS2);
  // G4LogicalVolume* schieldLogical = new G4LogicalVolume(schieldSolid,lead,"schieldLogical");
  // new G4PVPlacement(0, scheld_pos, schieldLogical, "schieldPhysical", logicWorld,
  //                   false,0,fCheckOverlaps);

  G4Tubs* schieldSolid
    = new G4Tubs("schieldSolid", schieldINXY, schieldXY, 0.5*scheld_thick, startAngle, spanningAngle);
  G4LogicalVolume* schieldLogical = new G4LogicalVolume(schieldSolid,lead,"schieldLogical");
  new G4PVPlacement(0, scheld_pos, schieldLogical, "schieldPhysical", logicWorld,
                    false,0,fCheckOverlaps);

  //
  // Visualization attributes
  //

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.0,0.33,0.0,1));
  logicscintP->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.333333,0,0.498039,1));
  schieldLogical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0,0.0,0.0,1));
  logictar->SetVisAttributes(visAttributes);
  logictarlayer1->SetVisAttributes(visAttributes);
  logictarlayer2->SetVisAttributes(visAttributes);
  logictarlayer3->SetVisAttributes(visAttributes);

  logicWorld->SetVisAttributes(G4VisAttributes::Invisible);
  logicscint->SetVisAttributes(G4VisAttributes::Invisible);
  logickiller->SetVisAttributes(G4VisAttributes::Invisible);

  // G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  // simpleBoxVisAtt->SetVisibility(true);
  // logicscint->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //

  return physWorld;
}
