#ifndef G4BertiniElectroNuclearBuilder_h
#define G4BertiniElectroNuclearBuilder_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4TheoFSGenerator.hh"
#include "G4GeneratorPrecompoundInterface.hh"
#include "G4QGSModel.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSMFragmentation.hh"
#include "G4ExcitedStringDecay.hh"

#include "G4CascadeInterface.hh"
#include "G4ElectroVDNuclearModel.hh"
//#include "G4ElectroNuclearReaction.hh"
#include "G4PhotoNuclearProcess.hh"
#include "G4ElectronNuclearProcess.hh"
#include "G4PositronNuclearProcess.hh"

//A. Dotti (June2013): No need to change this class for MT
// Since each thread owns its own instance (created by G4EmExtraPhysics)

class G4BertiniElectroNuclearBuilder 
{
  public: 
    G4BertiniElectroNuclearBuilder();
    virtual ~G4BertiniElectroNuclearBuilder();

  public: 
    virtual void Build();

  protected:
    G4PhotoNuclearProcess * thePhotoNuclearProcess;
    G4ElectronNuclearProcess * theElectronNuclearProcess;
    G4PositronNuclearProcess * thePositronNuclearProcess;
    G4ElectroVDNuclearModel * theElectroReaction;
    G4CascadeInterface * theGammaReaction;  
    
    G4TheoFSGenerator * theModel;
    G4GeneratorPrecompoundInterface * theCascade;
    G4QGSModel< G4GammaParticipants > * theStringModel;
    G4QGSMFragmentation * theFragmentation;
    G4ExcitedStringDecay * theStringDecay;
    G4bool wasActivated;
};

// 2002 by J.P. Wellisch

#endif

