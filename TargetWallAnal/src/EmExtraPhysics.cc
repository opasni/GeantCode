#include "G4EmExtraPhysics.hh"

#include "G4SystemOfUnits.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

#include "G4SynchrotronRadiation.hh"
#include "BertiniElectroNuclearBuilder.hh"
#include "G4MuonNuclearProcess.hh"
#include "G4MuonVDNuclearModel.hh"

#include "G4GammaConversionToMuons.hh"
#include "G4AnnihiToMuPair.hh"
#include "G4eeToHadrons.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4AutoDelete.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//
G4_DECLARE_PHYSCONSTR_FACTORY(G4EmExtraPhysics);

G4bool G4EmExtraPhysics::gnActivated  = true;
G4bool G4EmExtraPhysics::munActivated = true;
G4bool G4EmExtraPhysics::synActivated = false;
G4bool G4EmExtraPhysics::synActivatedForAll = false;
G4bool G4EmExtraPhysics::gmumuActivated = false;
G4bool G4EmExtraPhysics::pmumuActivated = false;
G4bool G4EmExtraPhysics::phadActivated = false;

G4ThreadLocal G4BertiniElectroNuclearBuilder* G4EmExtraPhysics::theGNPhysics = nullptr;
G4ThreadLocal G4SynchrotronRadiation* G4EmExtraPhysics::theSynchRad = nullptr;
G4ThreadLocal G4GammaConversionToMuons* G4EmExtraPhysics::theGammaToMuMu = nullptr;
G4ThreadLocal G4AnnihiToMuPair* G4EmExtraPhysics::thePosiToMuMu = nullptr;
G4ThreadLocal G4eeToHadrons* G4EmExtraPhysics::thePosiToHadrons = nullptr;

G4EmExtraPhysics::G4EmExtraPhysics(G4int ver):
  G4VPhysicsConstructor("G4GammaLeptoNuclearPhys"),
  verbose(ver)
{
  theMessenger = new G4EmMessenger(this);
  SetPhysicsType(bEmExtra);
  if(verbose > 1) G4cout << "### G4EmExtraPhysics" << G4endl;
}

G4EmExtraPhysics::G4EmExtraPhysics(const G4String&)
  : G4EmExtraPhysics(1)
{}

G4EmExtraPhysics::~G4EmExtraPhysics()
{
  delete theMessenger;
  theMessenger = nullptr;
}

void G4EmExtraPhysics::Synch(G4bool val)
{
  synActivated = val;
}

void G4EmExtraPhysics::SynchAll(G4bool val)
{
  synActivatedForAll = val;
  if(synActivatedForAll) { synActivated = true; }
}

void G4EmExtraPhysics::GammaNuclear(G4bool val)
{
  gnActivated = val;
}

void G4EmExtraPhysics::MuonNuclear(G4bool val)
{
  munActivated = val;
}

void G4EmExtraPhysics::GammaToMuMu(G4bool val)
{
  gmumuActivated = val;
}

void G4EmExtraPhysics::PositronToMuMu(G4bool val)
{
  pmumuActivated = val;
}

void G4EmExtraPhysics::PositronToHadrons(G4bool val)
{
  phadActivated = val;
}

void G4EmExtraPhysics::ConstructParticle()
{
  G4Gamma::Gamma();
  G4Electron::Electron();
  G4Positron::Positron();
  G4MuonPlus::MuonPlus();
  G4MuonMinus::MuonMinus();
}

void G4EmExtraPhysics::ConstructProcess()
{
  G4ParticleDefinition* gamma = G4Gamma::Gamma();
  G4ParticleDefinition* electron = G4Electron::Electron();
  G4ParticleDefinition* positron = G4Positron::Positron();
  G4ParticleDefinition* muonplus = G4MuonPlus::MuonPlus();
  G4ParticleDefinition* muonminus = G4MuonMinus::MuonMinus();

  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
  if(gnActivated) {
    theGNPhysics = new G4BertiniElectroNuclearBuilder();
    theGNPhysics->Build();
    //G4AutoDelete::Register(theGNPhysics);
  }
  if(munActivated) {
    G4MuonNuclearProcess* muNucProcess = new G4MuonNuclearProcess();
    G4MuonVDNuclearModel* muNucModel = new G4MuonVDNuclearModel();
    muNucProcess->RegisterMe(muNucModel);
    ph->RegisterProcess( muNucProcess, muonplus);
    ph->RegisterProcess( muNucProcess, muonminus);
  }
  if(gmumuActivated) {
    theGammaToMuMu = new G4GammaConversionToMuons();
    ph->RegisterProcess(theGammaToMuMu, gamma);
  }
  if(pmumuActivated) {
    thePosiToMuMu = new G4AnnihiToMuPair();
    ph->RegisterProcess(thePosiToMuMu, positron);
  }
  if(phadActivated) {
    thePosiToHadrons = new G4eeToHadrons();
    ph->RegisterProcess(thePosiToHadrons, positron);
  }
  if(synActivated) {
    theSynchRad = new G4SynchrotronRadiation();
    ph->RegisterProcess( theSynchRad, electron);
    ph->RegisterProcess( theSynchRad, positron);
    //G4AutoDelete::Register(theSynchRad);
    if(synActivatedForAll) {
      auto myParticleIterator=GetParticleIterator();
      myParticleIterator->reset();
      G4ParticleDefinition* particle = nullptr;

      while( (*myParticleIterator)() ) {
	particle = myParticleIterator->value();
	if( particle->GetPDGStable() && particle->GetPDGCharge() != 0.0) {
	  if(verbose > 1) {
	    G4cout << "### G4SynchrotronRadiation for "
		   << particle->GetParticleName() << G4endl;
	  }
	  ph->RegisterProcess( theSynchRad, particle);
	}
      }
    }
  }
}
