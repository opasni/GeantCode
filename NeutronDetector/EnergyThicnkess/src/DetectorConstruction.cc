#include "DetectorConstruction.hh"
// #include "MagneticField.hh"
#include "HadCalorimeterSD.hh"

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

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fcheckOverlaps(true),
  fMessenger(0),
  fHadCalScintiPV(0),
  fscintScintLogical(0),
  fVisAttributes(),
  fnofLayers(1),
  fnofLayersZ(1)
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



  //
  // World
  //

  G4bool checkOverlaps = fcheckOverlaps;
  G4double world_sizeX = 300*cm;
  G4double world_sizeY = 300*cm;
  G4double world_sizeZ  = 550*cm;
  fscintDetails = world_sizeZ/2;

  G4Box* solidWorld =
          new G4Box("World", 0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);
  G4LogicalVolume* worldLogical =
          new G4LogicalVolume(solidWorld, vacuum, "World");
  G4VPhysicalVolume* worldPhysical =
          new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World", 0, false, 0, checkOverlaps);


  G4double hz = 10*cm;
  G4double target_posz = 0.5*(-world_sizeZ+1*hz)+1*cm;

  //
  // Hadron Scintillator
  //

  G4double detectXY = 1*m;
  G4double detectINXY = 0.5000001*m;
  G4double detectZ = 1.25*m;

  G4double schieldthickness = 2*cm;

  G4double detectTarDist = 2.5*m;
  G4double detectR = -0.5*world_sizeZ+detectTarDist+detectZ;
  // fscintDetails = detectR - detectZ;
  G4double detsinR = 0;
   // (detectR-target_posz)*sin(rotTheta);
  G4double detcosR = (detectR-target_posz)+target_posz;
   // (detectR-target_posz)*cos(rotTheta)+target_posz;

  ConstructScintillator(detectXY, detectINXY, 0.5*detectZ, detsinR, detcosR,
                        0.5*schieldthickness, detectMat, worldLogical);

  //
  // Schield
  //

  // G4double schieldXY = detectINXY;
  // G4double schieldINXY = schieldXY - schieldthickness;
  // G4double schieldZ = 0.25*m + 0.5*schieldthickness;
  //
  // G4double schieldR = -0.5*world_sizeZ+detectTarDist+detectZ;
  //
  // G4double schsinR = 0;
  //  // (schieldR-target_posz)*sin(rotTheta);
  // G4double schcosR = schieldR;
  //  // (schieldR-target_posz)*cos(rotTheta)+target_posz;
  //
  // ConstructSchield(schieldXY, schieldINXY, schieldZ, schsinR, schcosR,
  //                       lead, worldLogical);



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
    = new HadCalorimeterSD(SDname="/HadCalorimeter", fnofLayers, fnofLayersZ);
  SDman->AddNewDetector(hadCalorimeter);
  fscintScintLogical->SetSensitiveDetector(hadCalorimeter);


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
  G4double z, a, aH, aC, density, hcratio, invhcr;
  G4double fractionmassH, fractionmassC;
  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole, density= universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

  aH = 1.01*g/mole; aC = 12.01*g/mole;
  G4Element* elH  = new G4Element("Hydrogen", "H", z= 1., aH);
  G4Element* elC  = new G4Element("Carbon"  , "C", z= 6., aC);

  // Liquid EJ301
  density= 0.874*g/cm3; hcratio = 1.212;
  invhcr = 1/hcratio;
  fractionmassH = (hcratio*aH)/(hcratio*aH+aC);
  fractionmassC = (invhcr*aC)/(invhcr*aC+aH);
  G4Material* mat_ej301 = new G4Material("EJ301", density, 2);
  // mat_ej301->AddElement(elH, fractionmassH*perCent);
  mat_ej301->AddElement(elH, fractionmassH);
  mat_ej301->AddElement(elC, fractionmassC);

  // Plastic EJ323
  density= 1.032*g/cm3; hcratio = 1.102;
  invhcr = 1/hcratio;
  fractionmassH = (hcratio*aH)/(hcratio*aH+aC);
  fractionmassC = (invhcr*aC)/(invhcr*aC+aH);
  G4Material* mat_ej323 = new G4Material("EJ323", density, 2);
  mat_ej323->AddElement(elH, fractionmassH);
  mat_ej323->AddElement(elC, fractionmassC);

  // Plastic EJ200
  density= 1.032*g/cm3; hcratio = 1.104;
  invhcr = 1/hcratio;
  fractionmassH = (hcratio*aH)/(hcratio*aH+aC);
  fractionmassC = (invhcr*aC)/(invhcr*aC+aH);
  G4Material* mat_ej200 = new G4Material("EJ200", density, 2);
  mat_ej200->AddElement(elH, fractionmassH);
  mat_ej200->AddElement(elC, fractionmassC);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



void DetectorConstruction::ConstructScintillator(G4double detectXY, G4double detectINXY, G4double detectZ,
                                          G4double detsinR, G4double detcosR, G4double leadThickness,
                                          std::vector<G4Material*> detectMat, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  G4int nofLayers = fnofLayers;
  G4Material* polyeth = detectMat[0];
  G4Material* vacuum = detectMat[1];
  G4double scintplateThick = (detectZ/fnofLayersZ - leadThickness);

  G4ThreeVector detectplace = G4ThreeVector(detsinR,0.,detcosR);

  G4VSolid* scintSolid = new G4Box("Box #1",detectXY,detectXY,detectZ);
  // G4VSolid* box2 = new G4Box("Box #2",detectINXY,detectINXY,detectZ);

  // G4VSolid* scintSolid = new G4SubtractionSolid("Schield", box1, box2);
  //G4VSolid* scintCalorimeterSolid = new G4Box("scintCalorimeterBox", detectXY, detectXY, detectZ);
  G4LogicalVolume* scintLogical
    = new G4LogicalVolume(scintSolid,vacuum,"scintLogical");
  new G4PVPlacement(0,detectplace,scintLogical,
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
    = new G4Box("scintCellBox",detectXY/nofLayers, detectXY/nofLayers, detectZ/fnofLayersZ);
  G4LogicalVolume* scintCellLogical
    = new G4LogicalVolume(scintCellSolid,vacuum,"scintCellLogical");
  new G4PVReplica("scintCellPhysical",scintCellLogical,
                  scintRowLogical,kZAxis, fnofLayersZ, 2*detectZ/fnofLayersZ);

  // scintillator scint plates
  G4VSolid* scintScintSolid
    = new G4Box("scintScintBox",detectXY/nofLayers, detectXY/nofLayers, scintplateThick);
  //G4LogicalVolume* scintScintLogical
  fscintScintLogical
    = new G4LogicalVolume(scintScintSolid,polyeth,"scintScintLogical");
  new G4PVPlacement(0,G4ThreeVector(0.,0.,detectZ/fnofLayersZ - scintplateThick),fscintScintLogical,
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

}

void DetectorConstruction::ConstructSchield(G4double schieldXY, G4double schieldINXY, G4double schieldZ,
                                          G4double schsinR, G4double schcosR,
                                          G4Material* lead, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  G4ThreeVector schplace = G4ThreeVector(schsinR,0.,schcosR);

  G4VSolid* boxSb1 = new G4Box("BoxSb #1",schieldXY,schieldXY,schieldZ);
  G4VSolid* boxSb2 = new G4Box("BoxSb #2",schieldINXY,schieldINXY,schieldZ);

  G4VSolid* schieldSolidB = new G4SubtractionSolid("Schield Inner", boxSb1, boxSb2);
  G4LogicalVolume* schieldLogicalB
    = new G4LogicalVolume(schieldSolidB,lead,"schieldLogicalB");
  new G4PVPlacement(0,schplace,schieldLogicalB,
                    "schieldPhysicalB",worldLogical,
                    false,0,checkOverlaps);


  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0));
  visAttributes->SetVisibility(true);
  schieldLogicalB->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}
