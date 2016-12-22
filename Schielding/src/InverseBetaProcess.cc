#include "InverseBetaProcess.hh" 
#include "G4Electron.hh"
#include "G4ElectroNuclearCrossSection.hh"
#include "InverseBetaXS.hh"
#include <iostream>

InverseBetaProcess::
InverseBetaProcess(const G4String& processName)
  : G4HadronInelasticProcess( processName, G4Electron::Electron() )
{ 
  G4CrossSectionDataStore * theStore = GetCrossSectionDataStore();
  //theStore->AddDataSet(new G4ElectroNuclearCrossSection);
  theStore->AddDataSet(new InverseBetaXS);
} 

    
InverseBetaProcess::~InverseBetaProcess()
{}


void InverseBetaProcess::ProcessDescription(std::ostream& outFile) const
{
  outFile << "This process handles inelastic electron scattering from\n" 
          << "nuclei by invoking one or more hadronic models and one\n"
          << "or more hadronic cross sections.\n";
}
