
#include <iostream>

#include "G4SystemOfUnits.hh"
#include "G4HadTmpUtil.hh"
#include "InverseBetaXS.hh"

// factory
#include "G4CrossSectionFactory.hh"
//
G4_DECLARE_XS_FACTORY(InverseBetaXS);

static const G4double massM1 = 4.693; // Constant from cross section integration
static const G4double massM2 = 40.77; // Constant from cross section integration
static const G4double massM3 = -28.72; // Constant from cross section integration

static const G4double nucleonMass = 0.939;
static const G4double femtobarn = 0.001*picobarn;


InverseBetaXS::InverseBetaXS():G4VCrossSectionDataSet(Default_Name()), lastE(0), lastSig(0)
{
    nistmngr = G4NistManager::Instance();
}

InverseBetaXS::~InverseBetaXS()
{}

void
InverseBetaXS::CrossSectionDescription(std::ostream& outFile) const
{
  outFile << "InverseBetaModel handles the inverse beta process\n"
          << "of the e- on p to nu_e and n.\n";
}


G4bool InverseBetaXS::IsElementApplicable(const G4DynamicParticle* /*aParticle*/, G4int /*Z*/, const G4Material*)
{
    return true;
}


G4double InverseBetaXS::GetElementCrossSection(const G4DynamicParticle* aPart, G4int ZZ, const G4Material*)
{
  if ( ZZ != 1)  return 0.;                           // Računamo le za primer vodikovega atoma
  else
  {
   	G4double energyE = aPart->GetKineticEnergy()/GeV; // V našem primeru nas zanima le energija elektrona
    if (energyE == lastE)  return lastSig*femtobarn;  // Če je energija enaka energiji prejšnjega ne računaj
    else
    {
      lastE = energyE;                                // Predefiniramo za nadaljnje račune
      // Izračun preseka preko parametrizacije
      lastSig = pow(energyE,2) * pow(0.5*nucleonMass + energyE,-3) * (massM1 + energyE*(massM2 + massM3*energyE));
      if(lastSig<0.) lastSig = 0.;                    // V primeru težav
      return lastSig*femtobarn;                       // Vrnemo rezultat
    }
  }
}

///////////////////////*****************************************************///////////////////////////
