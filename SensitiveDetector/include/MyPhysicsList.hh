#ifndef MyPhysicsList_h
#define MyPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4CascadeInterface.hh"
#include "globals.hh"

#include "InverseBetaModel.hh"
#include "G4ElectronNuclearProcess.hh"

class G4VPhysicsConstructor;
//class BertiniElectroNuclearBuilder;

class MyPhysicsList : public G4VModularPhysicsList
{
public:

  MyPhysicsList();
  virtual ~MyPhysicsList();

  virtual void ConstructParticle();
  virtual void ConstructProcess();
  virtual void SetCuts();

  virtual void InverseBetaProcess(); 

private:

  G4double fCutForElectron;
  G4double fCutForProton;

  //static G4ThreadLocal BertiniElectroNuclearBuilder* theGNPhysics;

  G4VPhysicsConstructor*  fParticleList;


  G4VPhysicsConstructor*  fEmPhysicsList;
  G4VPhysicsConstructor*  fEmExPhysicsList;
  G4VPhysicsConstructor*  fIonPhysicsList;
  G4VPhysicsConstructor* fHadronElasticPhysicsList;
  G4VPhysicsConstructor* fHadronPhysicsList;

  G4ElectronNuclearProcess * theElectronNuclearProcess;
  InverseBetaModel * theElectroReaction;    

};

#endif