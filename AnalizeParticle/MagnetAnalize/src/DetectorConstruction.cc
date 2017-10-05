#include "DetectorConstruction.hh"
#include "MagneticField.hh"
//#include "ParameterisationPHI.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UniformElectricField.hh"
#include "G4UniformMagField.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
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

#include "G4UserLimits.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(G4double magstr)
 : G4VUserDetectorConstruction(),
   fMessenger(0),
   fAbsorberPV(0),
   fMagneticLogical(0),
   fCheckOverlaps(true),
   fMagfieldstr(magstr)
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
  nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4double z, a, density;
  G4int ncomponents, natoms;
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
  G4Material* tarwall_mat;
  G4Material* tarwind_mat;

  tarwall_mat = G4Material::GetMaterial("G4_Al");
  tarwind_mat = G4Material::GetMaterial("G4_Be");

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

  G4double scint_Rmin = 250*cm;
  G4double scint_Rmax = 255*cm;
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
    // Magnet
    //

  G4double magZR = 0.4*m;
  G4double magTarDist = 0.1*m;
  G4double magR = 0.5*magZR+0.5*hz+magTarDist;

  G4ThreeVector magplace = G4ThreeVector(0.,0.,magR);
  G4RotationMatrix* fieldRotMag = new G4RotationMatrix();
  fieldRotMag->rotateX(pi/2);

  // Box Solid magnet
  G4VSolid* magneticSolid
    = new G4Box("Magnet", 0.5*magZR, 0.5*magZR, 0.5*magZR);

  // Cons solid magnet
  // G4double magR1Min, magR1Max, magR2Min, magR2Max;
  // G4double angMin = 0.162;
  // G4double angMax = 0.515;
  // magR1Min = magTarDist*tan(angMin); magR2Min = (magTarDist+magZR)*tan(angMin);
  // magR2Max = (magTarDist+hz+magZR)*tan(angMax); magR1Max = magR2Max;
  // G4Cons* magneticSolid
  //   = new G4Cons("Magnet", magR1Min, magR1Max, magR2Min, magR2Max, 0.5*magZR, startAngle, spanningAngle);

  fMagneticLogical = new G4LogicalVolume(magneticSolid, defaultMaterial, "magneticLogical");
  new G4PVPlacement(0,magplace,fMagneticLogical,
                    "magneticPhysical",logicWorld,
                    false,0,fCheckOverlaps);

  // set step limit in tube with magnetic field
  G4UserLimits* userLimits = new G4UserLimits(0.1*m);
  fMagneticLogical->SetUserLimits(userLimits);

  // G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));   // LightGray
  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.0,0.33,0.0,1));
  logicscintP->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0,0.0,0.0,1));
  logictar->SetVisAttributes(visAttributes);
  logictarlayer1->SetVisAttributes(visAttributes);
  logictarlayer2->SetVisAttributes(visAttributes);
  logictarlayer3->SetVisAttributes(visAttributes);


  //
  // Visualization attributes
  //
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(false);
  logicscint->SetVisAttributes(simpleBoxVisAtt);

  //
  // Always return the physical World
  //

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  // sensitive detectors -----------------------------------------------------
  // G4SDManager* SDman = G4SDManager::GetSDMpointer();
  // G4String SDname;

   // magnetic field ----------------------------------------------------------
  fMagneticField = new MagneticField();
  fMagneticField->SetField(fMagfieldstr*tesla);
  fFieldMgr = new G4FieldManager();
  fFieldMgr->SetDetectorField(fMagneticField);
  fFieldMgr->CreateChordFinder(fMagneticField);
  G4bool forceToAllDaughters = true;
  fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

  // Register the field and its manager for deleting
  G4AutoDelete::Register(fMagneticField);
  G4AutoDelete::Register(fFieldMgr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
