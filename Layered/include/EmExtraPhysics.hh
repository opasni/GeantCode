#ifndef G4EmExtraPhysics_h
#define G4EmExtraPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

#include "G4EmMessenger.hh"

class G4BertiniElectroNuclearBuilder;
class G4SynchrotronRadiation;
class G4GammaConversionToMuons;
class G4AnnihiToMuPair;
class G4eeToHadrons;

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
  void GammaToMuMu(G4bool val);
  void PositronToMuMu(G4bool val);
  void PositronToHadrons(G4bool val);

private:

  static G4bool gnActivated;
  static G4bool munActivated;
  static G4bool synActivated;
  static G4bool synActivatedForAll;
  static G4bool gmumuActivated;
  static G4bool pmumuActivated;
  static G4bool phadActivated;

  static G4ThreadLocal G4BertiniElectroNuclearBuilder* theGNPhysics;
  static G4ThreadLocal G4SynchrotronRadiation* theSynchRad;
  static G4ThreadLocal G4GammaConversionToMuons* theGammaToMuMu;
  static G4ThreadLocal G4AnnihiToMuPair* thePosiToMuMu;
  static G4ThreadLocal G4eeToHadrons* thePosiToHadrons;

  G4EmMessenger* theMessenger;
  G4int verbose;
};

#endif
