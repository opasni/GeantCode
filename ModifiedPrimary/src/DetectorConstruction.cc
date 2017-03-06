#include "DetectorConstruction.hh"
#include "MagneticField.hh"
#include "HadCalorimeterSD.hh"
#include "Parameterisation.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fcheckOverlaps(true),
  fMessenger(0),
  fHadCalScintiPV(0),
  fscintScintLogical(0),
  fMagneticLogical(0),
  fVisAttributes(),
  fnofLayers(20)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
    delete fMessenger;
    for (G4int i=0; i<G4int(fVisAttributes.size()); ++i)
    {
      delete fVisAttributes[i];
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Construct materials
  ConstructMaterials();

  G4Material* vacuum = G4Material::GetMaterial("Galactic");

  G4Material* polyeth = G4Material::GetMaterial("G4_POLYETHYLENE");
  G4Material* lead = G4Material::GetMaterial("G4_Pb");

  std::vector<G4Material*> detectMat(3,polyeth);
  detectMat[1]=vacuum;
  detectMat[2]=lead;

  //G4Material* aluminium = G4Material::GetMaterial("Galactic");
  G4Material* tarwall_mat = G4Material::GetMaterial("G4_Al");
  G4Material* tarwind_mat = G4Material::GetMaterial("G4_Be");

  std::vector<G4Material*> tarMat(3,vacuum);
  tarMat[1]=tarwall_mat;
  tarMat[2]=tarwind_mat;


  G4Material* lH2 = G4Material::GetMaterial("G4_lH2");

  //
  // World
  //

  G4bool checkOverlaps = fcheckOverlaps;
  G4double world_sizeX = 300*cm;
  G4double world_sizeY = 300*cm;
  G4double world_sizeZ  = 350*cm;

  G4Box* solidWorld =
          new G4Box("World", 0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);
  G4LogicalVolume* worldLogical =
          new G4LogicalVolume(solidWorld, vacuum, "World");
  G4VPhysicalVolume* worldPhysical =
          new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World", 0, false, 0, checkOverlaps);

  //
  // Target
  //

  G4double wall_thickness = 100*um;
  G4double window_thickness = 10*um;

  G4double outRlay1 = 2.5*cm;
  G4double hz = 10*cm;
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;
  G4double target_posz = 0.5*(-world_sizeZ+1*hz)+1*cm;

  // ConstructTarget(wall_thickness, window_thickness, outRlay1, hz, startAngle, spanningAngle,
  //                 target_posz, tarMat, lH2, worldLogical);

  //
  // Tube with Local Magnetic field
  //

  G4double rotTheta = 0.; // Specify rotation of magnetic tube and detector

  G4double magZR = 0.25*m;
  G4double magR = -0.5*world_sizeZ+0.5*magZR+0.5*hz+window_thickness+10*cm;

  G4double magsinR = (magR-target_posz)*sin(rotTheta);
  G4double magcosR = (magR-target_posz)*cos(rotTheta)+target_posz;

  // ConstructMagnet(magZR, magsinR, magcosR, vacuum, worldLogical);

  //
  // Hadron Scintillator
  //

  G4double detectXY = 1*m;
  G4double detectINXY = 0.5000001*m;
  G4double detectZ = 0.25*m;

  G4double schieldthickness = 2*cm;

  G4double detectTarDist = 2.5*m;
  G4double detectR = -0.5*world_sizeZ+detectTarDist+detectZ;
  fscintDetails = detectR - detectZ;
  G4double detsinR = (detectR-target_posz)*sin(rotTheta);
  G4double detcosR = (detectR-target_posz)*cos(rotTheta)+target_posz;

  G4int nofLayersZ = 3;

  ConstructScintillator(detectXY, detectINXY, detectZ, detsinR, detcosR,
                        0.5*schieldthickness, nofLayersZ, rotTheta, detectMat, worldLogical);

  //
  // Schield
  //

  G4double schieldXY = detectINXY;
  G4double schieldINXY = schieldXY - schieldthickness;
  G4double schieldZ = 0.25*m + 0.5*schieldthickness;

  G4double schieldR = -0.5*world_sizeZ+detectTarDist+detectZ;

  G4double schsinR = (schieldR-target_posz)*sin(rotTheta);
  G4double schcosR = (schieldR-target_posz)*cos(rotTheta)+target_posz;

  ConstructSchield2(schieldXY, schieldINXY, schieldZ, schsinR, schcosR, rotTheta,
                        lead, worldLogical);

  /*schieldXY = detectXY;
  schieldINXY = detectINXY;
  schieldZ = 0.5*schieldthickness;

  G4double schieldDetDist = 0.001*m;
  schieldR = -0.5*world_sizeZ+detectTarDist-schieldDetDist-schieldZ;

  schsinR = (schieldR-target_posz)*sin(rotTheta);
  schcosR = (schieldR-target_posz)*cos(rotTheta)+target_posz;

  ConstructSchield1(schieldXY, schieldINXY, schieldZ, schsinR, schcosR, rotTheta,
                        lead, worldLogical);*/


  //
  // World visualization attributes
  //

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  worldLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // return the world physical volume ----------------------------------------

  return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // sensitive detectors -----------------------------------------------------
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SDname;

  G4VSensitiveDetector* hadCalorimeter
    = new HadCalorimeterSD(SDname="/HadCalorimeter", fnofLayers);
  SDman->AddNewDetector(hadCalorimeter);
  fscintScintLogical->SetSensitiveDetector(hadCalorimeter);

   // magnetic field ----------------------------------------------------------
  fMagneticField = new MagneticField();
  fFieldMgr = new G4FieldManager();
  fFieldMgr->SetDetectorField(fMagneticField);
  fFieldMgr->CreateChordFinder(fMagneticField);
  G4bool forceToAllDaughters = true;
  // fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

  // Register the field and its manager for deleting
  G4AutoDelete::Register(fMagneticField);
  G4AutoDelete::Register(fFieldMgr);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructMaterials()
{
  G4NistManager* nistManager = G4NistManager::Instance();

  // Air
  nistManager->FindOrBuildMaterial("G4_AIR");

  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_lH2");
  nistManager->FindOrBuildMaterial("G4_Be");
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4double z, a, density;
  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole, density= universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructTarget(G4double wall_thickness,G4double window_thickness, G4double outRlay1,
                                          G4double hz, G4double startAngle, G4double spanningAngle,
                                          G4double target_posz, std::vector<G4Material*> tarMat,
                                          G4Material* lH2, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  G4ThreeVector tarpos = G4ThreeVector(0, 0, target_posz);
  G4Tubs* solidtar = new G4Tubs("Target", 0.*cm, outRlay1+wall_thickness, 0.5*hz+window_thickness, startAngle, spanningAngle);
  G4LogicalVolume* logictar = new G4LogicalVolume(solidtar, tarMat[0], "Target");
  new G4PVPlacement(0, tarpos, logictar, "Target", worldLogical, false, 0, checkOverlaps);

  G4ThreeVector laypos1 = G4ThreeVector(0, 0, 0);
  G4ThreeVector laypos2 = G4ThreeVector(0, 0, 0.5*(hz+window_thickness));
  G4ThreeVector laypos3 = G4ThreeVector(0, 0, -0.5*(hz+window_thickness));

  G4Tubs* solidtarlayer1
    = new G4Tubs("Target Wall", outRlay1, outRlay1 + wall_thickness, 0.5*hz, startAngle, spanningAngle);
  G4Tubs* solidtarlayer2
    = new G4Tubs("Target Wind 1", 0.*cm, outRlay1+wall_thickness, (0.5*window_thickness), startAngle, spanningAngle);
  G4Tubs* solidtarlayer3
    = new G4Tubs("Target Wind 2", 0.*cm, outRlay1+wall_thickness, (0.5*window_thickness), startAngle, spanningAngle);

  G4LogicalVolume* logictarlayer1
    = new G4LogicalVolume(solidtarlayer1, tarMat[1],
                     "Target Wall");
  G4LogicalVolume* logictarlayer2
    = new G4LogicalVolume(solidtarlayer2, tarMat[2],
                      "Target Wind 1");
  G4LogicalVolume* logictarlayer3
    = new G4LogicalVolume(solidtarlayer3, tarMat[2],
                      "Target Wind 2");

  new G4PVPlacement(0, laypos1, logictarlayer1,
                      "Target Wall", logictar, false, 0, checkOverlaps);
  new G4PVPlacement(0, laypos2, logictarlayer2,
                      "Target Wind 1", logictar, false, 0, checkOverlaps);
  new G4PVPlacement(0, laypos3, logictarlayer3,
                      "Target Wind 2", logictar, false, 0, checkOverlaps);

  // Target True
  G4ThreeVector pos1a = G4ThreeVector(0, 0, 0);
  G4Tubs* solidtarget = new G4Tubs("Target Center", 0.*cm, outRlay1, 0.5*hz,
                                    startAngle, spanningAngle);
  G4LogicalVolume* logictarget = new G4LogicalVolume(solidtarget, lH2, "Target Center");
  new G4PVPlacement(0, pos1a, logictarget,
                      "Target Center", logictar, false, 0, checkOverlaps);

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.8888,0.0,0.0));
  logictarlayer1->SetVisAttributes(visAttributes);
  logictarlayer2->SetVisAttributes(visAttributes);
  logictarlayer3->SetVisAttributes(visAttributes);
  logictarget->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  logictar->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);
}

void DetectorConstruction::ConstructMagnet(G4double magZR, G4double magsinR, G4double magcosR,
                                          G4Material* vacuum, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  G4ThreeVector magplace = G4ThreeVector(magsinR,0.,magcosR);
  G4RotationMatrix* fieldRotMag = new G4RotationMatrix();
  fieldRotMag->rotateX(pi/2);


  //G4VSolid* magneticSolid
  //  = new G4Tubs("magneticTubs",0.,magZR,magZR,0.,2*pi);
  G4VSolid* magneticSolid
    = new G4Box("World", 0.5*magZR, 0.5*magZR, 0.5*magZR);
  fMagneticLogical = new G4LogicalVolume(magneticSolid, vacuum, "magneticLogical");

  //G4RotationMatrix* fieldRotMag = new G4RotationMatrix();
  //fieldRotMag->rotateX(90.*deg);
  fMagnetPV = new G4PVPlacement(fieldRotMag,magplace,fMagneticLogical,
                    "magneticPhysical",worldLogical,
                    false,0,checkOverlaps);

  // set step limit in tube with magnetic field
  G4UserLimits* userLimits = new G4UserLimits(0.1*m);
  fMagneticLogical->SetUserLimits(userLimits);

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));   // LightGray
  fMagneticLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}
void DetectorConstruction::ConstructScintillator(G4double detectXY, G4double detectINXY, G4double detectZ,
                                          G4double detsinR, G4double detcosR, G4double leadThickness, G4int nofLayersZ,
                                          G4double rotTheta, std::vector<G4Material*> detectMat, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  G4int nofLayers = fnofLayers;
  G4Material* polyeth = detectMat[0];
  G4Material* vacuum = detectMat[1];
  G4Material* lead = detectMat[2];
  G4double scintplateThick = (detectZ/nofLayersZ - leadThickness);

  G4ThreeVector detectplace = G4ThreeVector(detsinR,0.,detcosR);
  G4RotationMatrix* fieldRotDet = new G4RotationMatrix();
  fieldRotDet->rotateY(-rotTheta);


  G4VSolid* box1 = new G4Box("Box #1",detectXY,detectXY,detectZ);
  G4VSolid* box2 = new G4Box("Box #2",detectINXY,detectINXY,detectZ);

  G4VSolid* scintSolid = new G4SubtractionSolid("Schield", box1, box2);
  //G4VSolid* scintCalorimeterSolid = new G4Box("scintCalorimeterBox", detectXY, detectXY, detectZ);
  G4LogicalVolume* scintLogical
    = new G4LogicalVolume(scintSolid,vacuum,"scintLogical");
  new G4PVPlacement(fieldRotDet,detectplace,scintLogical,
                    "scintPhysical",worldLogical,
                    false,0,checkOverlaps);

  // scintillator calorimeter column
  G4VSolid* scintColumnSolid
    = new G4Box("scintColumnBox", detectXY/nofLayers, detectXY,detectZ);
  G4LogicalVolume* scintColumnLogical
    = new G4LogicalVolume(scintColumnSolid,vacuum,"scintColumnLogical");
  new G4PVReplica("scintColumnPhysical",scintColumnLogical,
                  scintLogical,kXAxis, nofLayers, 2*detectXY/nofLayers);

  // scintillator calorimeter row
  G4VSolid* scintRowSolid
    = new G4Box("scintRowBox",detectXY/nofLayers, detectXY/nofLayers, detectZ);
  G4LogicalVolume* scintRowLogical
    = new G4LogicalVolume(scintRowSolid,vacuum,"scintRowLogical");
  new G4PVReplica("scintRowPhysical",scintRowLogical,
                  scintColumnLogical,kYAxis, nofLayers, 2*detectXY/nofLayers);

  // scintillator calorimeter cell
  G4VSolid* scintCellSolid
    = new G4Box("scintCellBox",detectXY/nofLayers, detectXY/nofLayers, detectZ/nofLayersZ);
  G4LogicalVolume* scintCellLogical
    = new G4LogicalVolume(scintCellSolid,vacuum,"scintCellLogical");
  new G4PVReplica("scintCellPhysical",scintCellLogical,
                  scintRowLogical,kZAxis, nofLayersZ, 2*detectZ/nofLayersZ);

  // scintillator lead plates
  G4VSolid* scintLeadSolid
    = new G4Box("scintLeadBox",detectXY/nofLayers, detectXY/nofLayers, leadThickness);
  G4LogicalVolume* scintLeadLogical
    = new G4LogicalVolume(scintLeadSolid,lead,"scintLeadLogical");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,-detectZ/nofLayersZ + leadThickness),scintLeadLogical,
                    "scintLeadPhysical",scintCellLogical,
                    false,0,checkOverlaps);

  // scintillator scint plates
  G4VSolid* scintScintSolid
    = new G4Box("scintScintBox",detectXY/nofLayers, detectXY/nofLayers, scintplateThick);
  //G4LogicalVolume* scintScintLogical
  fscintScintLogical
    = new G4LogicalVolume(scintScintSolid,polyeth,"scintScintLogical");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,detectZ/nofLayersZ - scintplateThick),fscintScintLogical,
                    "scintScintPhysical",scintCellLogical,
                    false,0,checkOverlaps);


  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  visAttributes->SetVisibility(false);
  scintLogical->SetVisAttributes(visAttributes);
  scintColumnLogical->SetVisAttributes(visAttributes);
  scintRowLogical->SetVisAttributes(visAttributes);
  scintCellLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  visAttributes->SetVisibility(true);
  fscintScintLogical->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0));
  visAttributes->SetVisibility(true);
  scintLeadLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}

void DetectorConstruction::ConstructSchield1(G4double schieldXY, G4double schieldINXY, G4double schieldZ,
                                          G4double schsinR, G4double schcosR, G4double rotTheta,
                                          G4Material* lead, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  // G4double schieldINXY = 0.090000001*m;
  G4ThreeVector schplace = G4ThreeVector(schsinR,0.,schcosR);
  G4RotationMatrix* fieldRotDet = new G4RotationMatrix();
  fieldRotDet->rotateY(-rotTheta);


  G4VSolid* boxSa1 = new G4Box("BoxSa #1",schieldXY,schieldXY,schieldZ);
  G4VSolid* boxSa2 = new G4Box("BoxSa #2",schieldINXY,schieldINXY,schieldZ);

  G4VSolid* schieldSolidA = new G4SubtractionSolid("Schield Front", boxSa1, boxSa2);
  G4LogicalVolume* schieldLogicalA
    = new G4LogicalVolume(schieldSolidA,lead,"schieldLogicalA");
  new G4PVPlacement(fieldRotDet,schplace,schieldLogicalA,
                    "schieldPhysicalA",worldLogical,
                    false,0,checkOverlaps);


  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0));
  visAttributes->SetVisibility(true);
  schieldLogicalA->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}

void DetectorConstruction::ConstructSchield2(G4double schieldXY, G4double schieldINXY, G4double schieldZ,
                                          G4double schsinR, G4double schcosR, G4double rotTheta,
                                          G4Material* lead, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  G4ThreeVector schplace = G4ThreeVector(schsinR,0.,schcosR);
  G4RotationMatrix* fieldRotDet = new G4RotationMatrix();
  fieldRotDet->rotateY(-rotTheta);


  G4VSolid* boxSb1 = new G4Box("BoxSb #1",schieldXY,schieldXY,schieldZ);
  G4VSolid* boxSb2 = new G4Box("BoxSb #2",schieldINXY,schieldINXY,schieldZ);

  G4VSolid* schieldSolidB = new G4SubtractionSolid("Schield Inner", boxSb1, boxSb2);
  G4LogicalVolume* schieldLogicalB
    = new G4LogicalVolume(schieldSolidB,lead,"schieldLogicalB");
  new G4PVPlacement(fieldRotDet,schplace,schieldLogicalB,
                    "schieldPhysicalB",worldLogical,
                    false,0,checkOverlaps);


  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0));
  visAttributes->SetVisibility(true);
  schieldLogicalB->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}
