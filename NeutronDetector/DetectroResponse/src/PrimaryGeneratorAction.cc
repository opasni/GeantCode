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
   fParticleGun1(0),
  //  fParticleDef(particle),
   fParticleDef("gamma"),
  //  fParticleDef("neutronIB"),
   cutVal(1)
{
  G4int nofParticles1 = 1;
  fParticleGun1 = new G4ParticleGun(nofParticles1);

  // Define which particle
  //
  G4String cut = "";
  if (cutVal == 1) cut = "10";
  G4String partName, procName;
  if (fParticleDef == "gamma") {
    partName = fParticleDef;
    procName = "GET" + cut;
  }
  else if (fParticleDef == "neutronIB") {
    partName = "neutron";
    procName = "NIBET" + cut; // "Se"
  }
  else {
    partName = "neutron";
    procName = "NTarET" + cut;
  }
  // G4cout << partName << ' ' << procName << G4endl;
  G4ParticleDefinition* particleDefinition
    = G4ParticleTable::GetParticleTable()->FindParticle(partName);
  fParticleGun1->SetParticleDefinition(particleDefinition);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun1->SetParticleEnergy(110*MeV);
  fParticleGun1->SetParticlePolarization(G4ThreeVector(0.,0.,1.));
  //fParticleGun1->SetParticleTime(0.1*ns);

  std::ifstream infile2(procName, std::ios::in);
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

  fParticleGun1
    ->SetParticlePosition(G4ThreeVector(0.0, 0.0, -worldZHalfLength));
  if (fParticleDef == "gamma") GenerateGamma();
  else if (fParticleDef == "neutronIB") GenerateNeutronIB();
  else GenerateNeutronTar();
  fParticleGun1->GeneratePrimaryVertex(anEvent);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GenerateGamma(){
  G4double phi = CLHEP::twopi*G4UniformRand();
  while (1) {
    G4double theta = (0.515-0.165)*G4UniformRand()+0.165;
    G4double energy, prob;
    if (cutVal == 0) {            // From 0
      energy = std::pow((8.1418-0.03724)*G4UniformRand()+0.03724,-1.42857)*MeV;
      prob = G4UniformRand()*(0.12282*pow(energy+0.05,-1.7));
    }
    else {                        // From 10
      energy = std::pow((0.19883-0.03724)*G4UniformRand()+0.03724,-1.42857)*MeV;
      prob = G4UniformRand()*(50.5455*pow(energy+0.05,-1.7));
    }
    G4int nth = GetIntTheta(theta);
    G4double userprob = GetInterpProb(energy, nth);

    if (userprob > prob) {
      G4double px = std::sin(theta)*std::cos(phi);
      G4double py = std::sin(theta)*std::sin(phi);
      G4double pz = std::cos(theta);
      fParticleGun1->SetParticleEnergy(energy);
      fParticleGun1
        ->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
      break;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GenerateNeutronIB(){
  G4double phi = CLHEP::twopi*G4UniformRand();
  G4double energyprobbord = 0.074246;
  while (1) {
    G4double theta = (0.515-0.165)*G4UniformRand()+0.165;

    // Energy dist form two parts, first choose one
    G4double enprobpart = G4UniformRand();
    G4double energy;
    G4double scale = energyprobbord/(3*(1-energyprobbord));
    if (enprobpart <= energyprobbord) energy = 15*G4UniformRand()*MeV;
    else {energy = (21 - 15)*G4UniformRand()*MeV + 15*MeV; scale=1;}

    G4int nth = GetIntTheta(theta);
    G4double userprob = GetInterpProb(energy, nth);
    G4double prob = G4UniformRand();
    if (userprob > (prob*scale)) {
      G4double px = std::sin(theta)*std::cos(phi);
      G4double py = std::sin(theta)*std::sin(phi);
      G4double pz = std::cos(theta);
      fParticleGun1->SetParticleEnergy(energy);
      fParticleGun1
        ->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
      break;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GenerateNeutronTar(){
  G4double phi = CLHEP::twopi*G4UniformRand();
  G4double energyhighval = 0.000003445;
  while (1) {
    G4double theta = (0.515-0.165)*G4UniformRand()+0.165;
    G4double energy, prob, base, energylowval;
    if (cutVal == 0) {            // From 0
      energylowval = 1.0;
      base = log(1.12);
      energy = -(log((energylowval-energyhighval)*G4UniformRand()+energyhighval)/base)*MeV;
      prob = G4UniformRand()*pow(1.12, -energy);
    }
    else {                        // From 10
      energylowval = 0.38554;
      base = log(1.1);
      energy = -(log((energylowval-energyhighval)*G4UniformRand()+energyhighval)/base)*MeV;
      prob = G4UniformRand()*2.5937*pow(1.15, -energy);
    }
    G4int nth = GetIntTheta(theta);
    G4double userprob = GetInterpProb(energy, nth);
    if (userprob > prob) {
      G4double px = std::sin(theta)*std::cos(phi);
      G4double py = std::sin(theta)*std::sin(phi);
      G4double pz = std::cos(theta);
      fParticleGun1->SetParticleEnergy(energy);
      fParticleGun1
        ->SetParticleMomentumDirection(G4ThreeVector(px, py, pz));
      break;
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int PrimaryGeneratorAction::GetIntTheta(G4double val)
{
  std::vector <G4double>* data = &fthetas;                // Vektor z kotami v središčih intervalov
    // Preverimo če smo pred prvim ali za zadnjim kotnim intervalom
  if (val < data->at(0)) return 0;
  else if (val > data->at(fnumY-1)) return fnumY-1;
    // Pozicijo preprosto izračunamo iz širine intervala. Preverimo kateremu intervalu smo bližje
  double delta = (val - data->at(0))/fdnY;
  G4int nth = (G4int) delta;
  if ((delta - nth) < 0.5) return nth;
  else return nth+1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4double PrimaryGeneratorAction::GetInterpProb(G4double energy, G4int nth) {
  G4double k, n;
    // Preverimo če smo pred prvim ali za zadnjim energijskim intervalom
  if(energy < fenergys[0]) return fvalues[nth][0];
  if (energy > fenergys[fnumX-1]) return fvalues[nth][fnumX-1];
    // Izračunamo katera v vrsti sta sosednja intervala
  double delta = (energy - fenergys[0])/fdnX;
  G4int out = (G4int) delta;
    // Sosednjima podatkoma prilagodimo premico (izračunamo naklon k in začetno vrednost n)
  k = (fvalues[nth][out]-fvalues[nth][out+1])/(fenergys[out]-fenergys[out+1]);
  n = (fvalues[nth][out+1]*fenergys[out]-fvalues[nth][out]*fenergys[out+1])/(fenergys[out]-fenergys[out+1]);
  G4double probab = (k*energy + n);
  return probab;
}
