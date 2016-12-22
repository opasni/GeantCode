#include "G4DecayPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronElasticPhysics.hh"

#include "MyPhysicsList.hh"

#include "G4SystemOfUnits.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4AutoDelete.hh"
 
#include "G4PhysicsConstructorFactory.hh"
#include "G4LossTableManager.hh"
#include "G4ProcessManager.hh"

MyPhysicsList::MyPhysicsList()
{
  G4LossTableManager::Instance();
  defaultCutValue = 0.7*mm;
  fCutForElectron  = defaultCutValue;
  fCutForProton    = defaultCutValue;
  verboseLevel    = 1;
  SetVerboseLevel(verboseLevel);

  // Particles
  fParticleList = new G4DecayPhysics("decays");
  fEmPhysicsList = new G4EmStandardPhysics();
  fEmExPhysicsList = new G4EmExtraPhysics();
  fIonPhysicsList = new G4IonPhysics();
  fHadronElasticPhysicsList = new G4HadronElasticPhysics();
  //fHadronPhysicsList = new G4HadronPhysicsQGSP_BERT_HP();
  fHadronPhysicsList = new G4HadronPhysicsQGSP_BERT();


}    

MyPhysicsList::~MyPhysicsList() 
{  
  delete fParticleList;
  delete fEmPhysicsList;
  delete fEmExPhysicsList;
  delete fIonPhysicsList;
  delete fHadronElasticPhysicsList;
  delete fHadronPhysicsList;
}

void MyPhysicsList::ConstructParticle()
{
  fParticleList->ConstructParticle();
}

//#include "G4EmProcessOptions.hh"

void MyPhysicsList::ConstructProcess()
{
  InverseBetaProcess();

  AddTransportation();

  fEmPhysicsList->ConstructProcess();

  fParticleList->ConstructProcess();
  fEmExPhysicsList->ConstructProcess();
  fHadronElasticPhysicsList->ConstructProcess();
  fHadronPhysicsList->ConstructProcess();
  fIonPhysicsList->ConstructProcess();
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MyPhysicsList::SetCuts()
{
  SetCutValue(fCutForElectron, "e-");
  SetCutValue(fCutForProton, "proton");
}


void MyPhysicsList::InverseBetaProcess()
{
  
  G4ProcessManager * aProcMan = 0;

  theElectronNuclearProcess = new G4ElectronNuclearProcess;
  theElectroReaction = new InverseBetaModel;
  
  aProcMan = G4Electron::Electron()->GetProcessManager();
  theElectronNuclearProcess->RegisterMe(theElectroReaction);
  //theElectronNuclearProcess->MultiplyCrossSectionBy(pow(10,13));

  aProcMan->AddDiscreteProcess(theElectronNuclearProcess);

}