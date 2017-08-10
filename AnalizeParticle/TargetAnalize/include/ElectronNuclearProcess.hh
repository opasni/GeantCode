#ifndef G4ElectronNuclearProcess_h
#define G4ElectronNuclearProcess_h 1
 
#include "G4HadronInelasticProcess.hh"
 
class G4ElectronNuclearProcess : public G4HadronInelasticProcess
{
public:
    
  G4ElectronNuclearProcess(const G4String& processName = "electronNuclear" );
  virtual ~G4ElectronNuclearProcess();

  virtual void ProcessDescription(std::ostream& outFile) const;
};

#endif

