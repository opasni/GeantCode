#include "PrimaryGeneratorAction.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SPSEneDistribution.hh"
#include "G4SPSPosDistribution.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
 // : G4VUserPrimaryGeneratorAction(),
{
  fParticleGun1 = new G4GeneralParticleSource();

  // fParticleGun1 = new G4GeneralParticleSource();
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* gama = particleTable->FindParticle(particleName="gamma");

  //....PARTICLE DEFINITIONS
  fParticleGun1->SetParticleDefinition(gama);

  //....NUMBER OF PARTICLES
  fParticleGun1->SetNumberOfParticles(100);

   // DEFINE A MONO-ENERGETIC SOURCE
   G4SPSEneDistribution *eneDist = fParticleGun1->GetCurrentSource()->GetEneDist() ;
   eneDist->SetEnergyDisType("Mono");
   eneDist->SetMonoEnergy(110.0*MeV);

   // SET POSITION DISTRIBUTION
   G4SPSPosDistribution *posDist = fParticleGun1->GetCurrentSource()->GetPosDist() ;
   posDist->SetPosDisType("Plane");
   posDist->SetPosDisShape("Circle");
   posDist->SetRadius(20.0*cm);



   G4SPSAngDistribution *angDist = fParticleGun1->GetCurrentSource()->GetAngDist() ;

   angDist->SetParticleMomentumDirection(G4ThreeVector(0.0,0.0,-1.0)) ;
   angDist->SetMinTheta(0.165) ;
   angDist->SetMaxTheta(0.515) ;
   posDist->SetCentreCoords(G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  fParticleGun1->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
