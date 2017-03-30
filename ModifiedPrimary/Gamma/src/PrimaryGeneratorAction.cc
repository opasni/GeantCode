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

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun1(0)
{
  G4int nofParticles1 = 1;
  fParticleGun1 = new G4ParticleGun(nofParticles1);

  // default particle kinematic
  //
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fParticleGun1->SetParticleDefinition(particleDefinition);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun1->SetParticleEnergy(110*MeV);
  fParticleGun1->SetParticlePolarization(G4ThreeVector(0.,0.,1.));
  //fParticleGun1->SetParticleTime(0.1*ns);

  // std::ifstream infile2("EnergyTheta", std::ios::in);
  std::ifstream infile2("EnergyTheta10", std::ios::in);
  infile2 >> fnumX >> fnumY >> fdnX >> fdnY;
  G4double ehi, thi, val;
  for (int i=0; i<fnumX; i++) {
    std::vector <G4double> vals;
    for (int j=0; j<fnumY; j++) {
      infile2 >> ehi >> thi >> val;
      vals.push_back(val);
      if(i==0) fenergys.push_back(ehi);
    }
    fthetas.push_back(thi);
    fvalues.push_back(vals);
  }
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

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore
  //
  G4double worldZHalfLength = 0;
  G4LogicalVolume* worlLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");

  G4Box* worldBox = 0;
  if ( worlLV) worldBox = dynamic_cast< G4Box*>(worlLV->GetSolid());
  if ( worldBox ) {
    worldZHalfLength = worldBox->GetZHalfLength();
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box not found." << G4endl;
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
      "MyCode0002", JustWarning, msg);
  }


  int i;
  int Nbeam = 1;
  for (i = 0; i < Nbeam; i++ ){
    G4double phi = CLHEP::twopi*G4UniformRand();
    fParticleGun1
      ->SetParticlePosition(G4ThreeVector(0.0, 0.0, -worldZHalfLength));
    while (1) {
      G4double theta = (0.515-0.165)*G4UniformRand()+0.165;
      G4double energy = EnergyProbDist(G4UniformRand())*MeV;
      G4int nth = GetIntTheta(theta);
      G4double userprob = GetInterpProb(energy, nth);
      // G4double prob = G4UniformRand()*(0.12282*pow(energy+0.05,-1.7)); // From 0
      G4double prob = G4UniformRand()*(50.5455*pow(energy+0.05,-1.7)); // From 10
      if (userprob > prob) {
        G4double px = std::sin(theta)*std::cos(phi);
        G4double py = std::sin(theta)*std::sin(phi);
        G4double pz = std::cos(theta);
        fParticleGun1->SetParticleEnergy(energy);
        fParticleGun1
          ->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
          fParticleGun1->GeneratePrimaryVertex(anEvent);
        break;
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int PrimaryGeneratorAction::GetIntTheta(G4double val)
{
  std::vector <G4double>* data = &fthetas;

  if (val < data->at(0)) return 0;
  else if (val > data->at(fnumY-1)) return fnumY-1;

  double delta = (val - data->at(0))/fdnY;
  G4int out = (G4int) delta;
  if ((delta - out) < 0.5) return out;
  else return out+1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::EnergyProbDist(G4double ran) {
  G4double val;
  // val = pow((pow(0.05,-0.7)-pow(0.1,-0.7)*ran+pow(0.1,-0.7)),-(1/0.7));
  // val = std::pow((8.1418-0.03724)*ran+0.03724,-1.42857); //From 0
  val = std::pow((0.19883-0.03724)*ran+0.03724,-1.42857); //From 10
  return val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::GetInterpProb(G4double energy, G4int nth) {
  G4double k, n;
  if(energy < fenergys[0]) return fvalues[nth][0];
  if (energy > fenergys[fnumX-1]) return fvalues[nth][fnumX-1];

  double delta = (energy - fenergys[0])/fdnX;
  G4int out = (G4int) delta;

  k = (fvalues[nth][out]-fvalues[nth][out+1])/(fenergys[out]-fenergys[out+1]);
  n = (fvalues[nth][out+1]*fenergys[out]-fvalues[nth][out]*fenergys[out+1])/(fenergys[out]-fenergys[out+1]);

  G4double probab = (k*energy + n);
  // G4cout << k << ' ' << energy << ' ' << n << G4endl;
  // G4cout << fvalues[nth][out] << ' ' << probab << ' ' << fvalues[nth][out+1] << G4endl;

  return probab;
}
