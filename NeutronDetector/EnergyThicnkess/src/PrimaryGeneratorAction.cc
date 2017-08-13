#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(G4String particle)
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun1(0)
{
  G4int nofParticles1 = 1;
  fParticleGun1 = new G4ParticleGun(nofParticles1);

  // default particle kinematic
  //
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle(particle);
  fParticleGun1->SetParticleDefinition(particleDefinition);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun1->SetParticleEnergy(110.*MeV);
  fParticleGun1->SetParticlePolarization(G4ThreeVector(0.,0.,-1.));
  //fParticleGun1->SetParticleTime(0.1*ns);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // This function is called at the begining of event

  G4double worldZHalfLength = 0;
  G4LogicalVolume* worlLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  G4Box* worldBox = 0;
  if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid());
  if ( worldBox ) {worldZHalfLength = worldBox->GetZHalfLength();}
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box not found." << G4endl;
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "MyCode0002", JustWarning, msg);
  }

  // int i;
  // int Nbeam = 208;
  // for (i = 0; i < Nbeam; i++ ){
  // G4double z = G4UniformRand()*100*um+4*cm;
  // Set gun position
  fParticleGun1
    ->SetParticlePosition(G4ThreeVector(0.0, 0.0, -worldZHalfLength));

  fParticleGun1->GeneratePrimaryVertex(anEvent);
  // }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
