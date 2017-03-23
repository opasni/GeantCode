#include "ElectronNuclearProcess.hh" 
#include "G4Electron.hh"
#include "G4ElectroNuclearCrossSection.hh"
#include "InverseBetaXS.hh"
#include <iostream>

G4ElectronNuclearProcess::
G4ElectronNuclearProcess(const G4String& processName)
  : G4HadronInelasticProcess( processName, G4Electron::Electron() )
{ 
  G4CrossSectionDataStore * theStore = GetCrossSectionDataStore();
  //theStore->AddDataSet(new G4ElectroNuclearCrossSection);
  theStore->AddDataSet(new InverseBetaXS);
} 

    
G4ElectronNuclearProcess::~G4ElectronNuclearProcess()
{}


void G4ElectronNuclearProcess::ProcessDescription(std::ostream& outFile) const
{
  outFile << "This process handles inelastic electron scattering from\n" 
          << "nuclei by invoking one or more hadronic models and one\n"
          << "or more hadronic cross sections.\n";
}
