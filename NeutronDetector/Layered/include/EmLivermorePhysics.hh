#ifndef G4EmLivermorePhysics_h
#define G4EmLivermorePhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4EmLivermorePhysics : public G4VPhysicsConstructor
{
public:

  explicit G4EmLivermorePhysics(G4int ver=1, const G4String& name="");

  virtual ~G4EmLivermorePhysics();

  virtual void ConstructParticle();
  virtual void ConstructProcess();

private:
  G4int  verbose;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
