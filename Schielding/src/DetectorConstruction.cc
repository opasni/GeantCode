#include "DetectorConstruction.hh"
#include "MagneticField.hh"

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

  //G4Material* magnet = G4Material::GetMaterial("G4_AIR");
  //G4Material* magnet_mat = G4Material::GetMaterial("Galactic");
  
  //G4Material* scint_mat = G4Material::GetMaterial("G4_POLYETHYLENE"); 
  G4Material* scint_mat = G4Material::GetMaterial("Galactic");

  G4Material* aluminium = G4Material::GetMaterial("G4_Al");
  
  G4Material* lead = G4Material::GetMaterial("G4_Pb");

  G4Material* lH2 = G4Material::GetMaterial("H2liquid");
  
  // Option to switch on/off checking of volumes overlaps
  //

  
  //
  // World
  // 

  G4bool checkOverlaps = fcheckOverlaps;
  G4double world_sizeX = 100*cm;
  G4double world_sizeY = 100*cm;
  G4double world_sizeZ  = 600*cm;

  G4Box* solidWorld = 
          new G4Box("World", 0.5*world_sizeX, 0.5*world_sizeY, 0.5*world_sizeZ);
  G4LogicalVolume* worldLogical = 
          new G4LogicalVolume(solidWorld, vacuum, "World");                                    
  G4VPhysicalVolume* worldPhysical = 
          new G4PVPlacement(0, G4ThreeVector(), worldLogical, "World", 0, false, 0, checkOverlaps);

  //
  // Target
  //

  G4double layer_thickness = 10*um;
  G4double outRlay1 = 2.5*cm;
  G4double hz = 10*cm;
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;
  G4double target_posz = 0.5*(-world_sizeZ+1*hz)+1*cm; 

  ConstructTarget(layer_thickness, outRlay1, hz, startAngle, spanningAngle, 
                  target_posz, vacuum, aluminium, lH2, worldLogical);

  //
  // Tube with Local Magnetic field
  //

  G4double rotTheta = 0.; // Specify rotation of magnetic tube and detector

  G4double magZR = 0.25*m;
  G4double magR = -0.5*world_sizeZ+0.5*magZR+0.5*hz+layer_thickness+10*cm;

  G4double magsinR = (magR-target_posz)*sin(rotTheta);
  G4double magcosR = (magR-target_posz)*cos(rotTheta)+target_posz;

  ConstructMagnet(magZR, magsinR, magcosR, vacuum, worldLogical);
  
  //
  // Hadron Scintillator
  //

  G4double detectXY = 0.5*m;
  G4double detectZ = 0.3*m;

  G4double detectTarDist = 2.5*m;
  G4double detectR = -0.5*world_sizeZ+detectTarDist+detectZ;

  G4double detsinR = (detectR-target_posz)*sin(rotTheta);
  G4double detcosR = (detectR-target_posz)*cos(rotTheta)+target_posz;

  ConstructScintillator(detectXY, detectZ, detsinR, detcosR, rotTheta,
                        scint_mat, worldLogical);
  
  //
  // Hadron Scintillator
  //
  

  G4double schieldXY = detectXY;
  G4double schieldZ = 5*cm;

  G4double schieldDetDist = 0.05*m;
  G4double schieldR = -0.5*world_sizeZ+detectTarDist-schieldDetDist-schieldZ;

  G4double schsinR = (schieldR-target_posz)*sin(rotTheta);
  G4double schcosR = (schieldR-target_posz)*cos(rotTheta)+target_posz;

  ConstructSchield(schieldXY, schieldZ, schsinR, schcosR, rotTheta,
                        lead, worldLogical);

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
   // magnetic field ----------------------------------------------------------
  fMagneticField = new MagneticField();
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

void DetectorConstruction::ConstructMaterials()
{
  G4NistManager* nistManager = G4NistManager::Instance();

  // Air 
  nistManager->FindOrBuildMaterial("G4_AIR");
  
  nistManager->FindOrBuildMaterial("G4_Al");
  nistManager->FindOrBuildMaterial("G4_Pb");
  nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4double z, a, density;  
  G4int ncomponents, natoms;
  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole, density= universe_mean_density, kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Liquid Hydrogen  
  G4Element* H  = new G4Element("Hydrogen" ,"H" , z= 1., a=   2.02*g/mole);
  G4Material* H2l = new G4Material("H2liquid", density= 70.85*kg/m3, ncomponents=1);
  H2l->AddElement(H, natoms=2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructTarget(G4double layer_thickness, G4double outRlay1,
                                          G4double hz, G4double startAngle, G4double spanningAngle,
                                          G4double target_posz, G4Material* vacuum, G4Material* aluminium,
                                          G4Material* lH2, G4LogicalVolume* worldLogical)
{
  G4bool checkOverlaps = fcheckOverlaps;
  G4ThreeVector tarpos = G4ThreeVector(0, 0, target_posz);
  G4Tubs* solidtar = new G4Tubs("Target", 0.*cm, outRlay1, 0.5*hz, startAngle, spanningAngle);
  G4LogicalVolume* logictar 
    = new G4LogicalVolume(solidtar, vacuum, "Target");
  new G4PVPlacement(0, tarpos, logictar, "Target", worldLogical, false, 0, checkOverlaps);
 
  G4ThreeVector laypos1
   = G4ThreeVector(0, 0, 0); 
  G4ThreeVector laypos2
   = G4ThreeVector(0, 0, 0.5*hz-(0.5*layer_thickness));
  G4ThreeVector laypos3
   = G4ThreeVector(0, 0, -0.5*hz+(0.5*layer_thickness));

  G4Tubs* solidtarlayer1
    = new G4Tubs("Target Layer 1", outRlay1-layer_thickness, outRlay1, 0.5*hz-layer_thickness, startAngle, spanningAngle);
  G4Tubs* solidtarlayer2
    = new G4Tubs("Target Layer 2", 0.*cm, outRlay1, (0.5*layer_thickness), startAngle, spanningAngle);
  G4Tubs* solidtarlayer3
    = new G4Tubs("Target Layer 3", 0.*cm, outRlay1, (0.5*layer_thickness), startAngle, spanningAngle);

  G4LogicalVolume* logictarlayer1
    = new G4LogicalVolume(solidtarlayer1, aluminium, 
                     "Target Layer 1");
  G4LogicalVolume* logictarlayer2
    = new G4LogicalVolume(solidtarlayer2, aluminium, 
                      "Target Layer 2");
  G4LogicalVolume* logictarlayer3
    = new G4LogicalVolume(solidtarlayer3, aluminium, 
                      "Target Layer 3");

  new G4PVPlacement(0, laypos1, logictarlayer1, 
                      "Target Layer 1", logictar, false, 0, checkOverlaps);
  new G4PVPlacement(0, laypos2, logictarlayer2, 
                      "Target Layer 2", logictar, false, 0, checkOverlaps);
  new G4PVPlacement(0, laypos3, logictarlayer3, 
                      "Target Layer 3", logictar, false, 0, checkOverlaps);
    
  // Target True
  G4ThreeVector pos1a = G4ThreeVector(0, 0, 0);
  G4Tubs* solidtarget
    = new G4Tubs("Target Center", 0.*cm, outRlay1-layer_thickness, (0.5*hz)-layer_thickness, startAngle, spanningAngle);
  G4LogicalVolume* logictarget
    = new G4LogicalVolume(solidtarget, lH2, "Target Center");
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
  new G4PVPlacement(fieldRotMag,magplace,fMagneticLogical,
                    "magneticPhysical",worldLogical,
                    false,0,checkOverlaps);

  // set step limit in tube with magnetic field  
  G4UserLimits* userLimits = new G4UserLimits(0.1*m);
  fMagneticLogical->SetUserLimits(userLimits);

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));   // LightGray
  fMagneticLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}
void DetectorConstruction::ConstructScintillator(G4double detectXY, G4double detectZ, G4double detsinR, 
                                          G4double detcosR, G4double rotTheta,
                                          G4Material* scint_mat, G4LogicalVolume* worldLogical)
{  
  G4bool checkOverlaps = fcheckOverlaps;
  G4double detectINXY = 0.10000001*m;
  G4ThreeVector detectplace = G4ThreeVector(detsinR,0.,detcosR);
  G4RotationMatrix* fieldRotDet = new G4RotationMatrix();
  fieldRotDet->rotateY(-rotTheta);


  G4VSolid* box1 = new G4Box("Box #1",detectXY,detectXY,detectZ);
  G4VSolid* box2 = new G4Box("Box #2",detectINXY,detectINXY,detectZ); 

  G4VSolid* scintSolid = new G4SubtractionSolid("Scintilator", box1, box2);
  //G4VSolid* scintCalorimeterSolid = new G4Box("scintCalorimeterBox", detectXY, detectXY, detectZ);
  G4LogicalVolume* scintLogical
    = new G4LogicalVolume(scintSolid,scint_mat,"scintLogical");
  fHadCalScintiPV = new G4PVPlacement(fieldRotDet,detectplace,scintLogical,
                    "scintPhysical",worldLogical,
                    false,0,checkOverlaps);
  

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
  scintLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}
void DetectorConstruction::ConstructSchield(G4double schieldXY, G4double schieldZ, G4double schsinR, 
                                          G4double schcosR, G4double rotTheta,
                                          G4Material* lead, G4LogicalVolume* worldLogical)
{  
  G4bool checkOverlaps = fcheckOverlaps;
  G4double schieldINXY = 0.090000001*m;
  G4ThreeVector schplace = G4ThreeVector(schsinR,0.,schcosR);
  G4RotationMatrix* fieldRotDet = new G4RotationMatrix();
  fieldRotDet->rotateY(-rotTheta);


  G4VSolid* boxS1 = new G4Box("BoxS #1",schieldXY,schieldXY,schieldZ);
  G4VSolid* boxS2 = new G4Box("BoxS #2",schieldINXY,schieldINXY,schieldZ); 

  G4VSolid* schieldSolid = new G4SubtractionSolid("Schield", boxS1, boxS2);
  G4LogicalVolume* schieldLogical
    = new G4LogicalVolume(schieldSolid,lead,"schieldLogical");
  new G4PVPlacement(fieldRotDet,schplace,schieldLogical,
                    "schieldPhysical",worldLogical,
                    false,0,checkOverlaps);
  

  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0));
  visAttributes = new G4VisAttributes(G4Colour(1.0, 0.5, 0.0));
  schieldLogical->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

}
