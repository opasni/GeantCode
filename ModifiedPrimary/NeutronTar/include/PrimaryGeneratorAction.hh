#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include <vector>

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gum.
///

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* event);

  // set methods
  void SetRandomFlag(G4bool value);

private:
  G4int GetIntTheta(G4double val);
  G4double EnergyProbDist(G4double ran);
  G4double GetInterpProb(G4double energy, G4int nth);

  G4ParticleGun*  fParticleGun1; // G4 particle gun 1
  std::vector <G4double> fthetas;
  std::vector <G4double> fenergys;
  std::vector <std::vector <G4double>> fvalues;
  G4int fnumX, fnumY;
  G4double fdnX, fdnY;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
