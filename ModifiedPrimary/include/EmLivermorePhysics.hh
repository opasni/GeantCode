#ifndef EmLivermorePhysics_h
#define EmLivermorePhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EmLivermorePhysics : public G4VPhysicsConstructor
{
public:

  EmLivermorePhysics(G4int ver = 1);

  // obsolete
  EmLivermorePhysics(G4int ver, const G4String& name);

  virtual ~EmLivermorePhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif






