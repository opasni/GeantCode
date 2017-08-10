#include "BertiniElectroNuclearBuilder.hh"

#include "globals.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4ProcessManager.hh"


G4BertiniElectroNuclearBuilder::G4BertiniElectroNuclearBuilder() :
    thePhotoNuclearProcess(0), theElectronNuclearProcess(0),
    thePositronNuclearProcess(0), theElectroReaction(0),
    theGammaReaction(0), theModel(0), theCascade(0),
    theStringModel(0), theFragmentation(0), theStringDecay(0),
   wasActivated(false)
{
}

G4BertiniElectroNuclearBuilder::~G4BertiniElectroNuclearBuilder()
{
  if(wasActivated) {
    delete theFragmentation;
    delete theStringDecay;
    //delete theStringModel;
    //delete thePhotoNuclearProcess;
    //delete theElectronNuclearProcess;
    //delete thePositronNuclearProcess;
    //delete theElectroReaction;
    //delete theGammaReaction;
    //delete theModel;
    //delete theCascade;
  }
}

void G4BertiniElectroNuclearBuilder::Build()
{
  if(wasActivated) return;
  wasActivated=true;

  thePhotoNuclearProcess = new G4PhotoNuclearProcess;
  theElectronNuclearProcess = new G4ElectronNuclearProcess;
  thePositronNuclearProcess = new G4PositronNuclearProcess;
  //  theElectroReaction = new G4ElectroNuclearReaction;
  theElectroReaction = new G4ElectroVDNuclearModel;
  theGammaReaction = new G4CascadeInterface;

  theModel = new G4TheoFSGenerator;

  theStringModel = new G4QGSModel< G4GammaParticipants >;
  theStringDecay = new G4ExcitedStringDecay(theFragmentation=new G4QGSMFragmentation);
  theStringModel->SetFragmentationModel(theStringDecay);

  theCascade = new G4GeneratorPrecompoundInterface;

  theModel->SetTransport(theCascade);
  theModel->SetHighEnergyGenerator(theStringModel);

  G4ProcessManager * aProcMan = 0;

  aProcMan = G4Gamma::Gamma()->GetProcessManager();
  theGammaReaction->SetMaxEnergy(3.5*GeV);
  thePhotoNuclearProcess->RegisterMe(theGammaReaction);
  theModel->SetMinEnergy(3.*GeV);
  theModel->SetMaxEnergy(100*TeV);
  thePhotoNuclearProcess->RegisterMe(theModel);
  thePhotoNuclearProcess->MultiplyCrossSectionBy(pow(10,6));
  aProcMan->AddDiscreteProcess(thePhotoNuclearProcess);

  aProcMan = G4Electron::Electron()->GetProcessManager();
  theElectronNuclearProcess->RegisterMe(theElectroReaction);
  theElectronNuclearProcess->MultiplyCrossSectionBy(pow(10,6));
  aProcMan->AddDiscreteProcess(theElectronNuclearProcess);

  aProcMan = G4Positron::Positron()->GetProcessManager();
  thePositronNuclearProcess->RegisterMe(theElectroReaction);
  aProcMan->AddDiscreteProcess(thePositronNuclearProcess);
}
