#ifndef G4EmExtraPhysics_h
#define G4EmExtraPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

#include "G4EmMessenger.hh"

class G4BertiniElectroNuclearBuilder;
class G4SynchrotronRadiation;

class G4EmExtraPhysics : public G4VPhysicsConstructor
{
public:

  G4EmExtraPhysics(G4int ver = 1);

  // obsolete
  G4EmExtraPhysics(const G4String& name);

  virtual ~G4EmExtraPhysics();

  void ConstructParticle();
  void ConstructProcess();

  void Synch(G4bool val);
  void SynchAll(G4bool val);
  void GammaNuclear(G4bool val);
  void MuonNuclear(G4bool val);

private:

  static G4bool gnActivated;
  static G4bool munActivated;
  static G4bool synActivated;
  static G4bool synActivatedForAll;

  static G4ThreadLocal G4BertiniElectroNuclearBuilder* theGNPhysics;
  static G4ThreadLocal G4SynchrotronRadiation* theSynchRad;

  G4EmMessenger* theMessenger;
  G4int verbose;
};

#endif





