#ifndef InverseBetaProcess_h
#define InverseBetaProcess_h 1
 
#include "G4HadronInelasticProcess.hh"
 
class InverseBetaProcess : public G4HadronInelasticProcess
{
public:
    
  InverseBetaProcess(const G4String& processName = "InverseBeta" );
  virtual ~InverseBetaProcess();

  virtual void ProcessDescription(std::ostream& outFile) const;
};

#endif

