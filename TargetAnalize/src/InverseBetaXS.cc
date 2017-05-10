
#include <iostream>

#include "G4SystemOfUnits.hh"
#include "G4HadTmpUtil.hh"
#include "InverseBetaXS.hh"

// factory
#include "G4CrossSectionFactory.hh"
//
G4_DECLARE_XS_FACTORY(InverseBetaXS);

static const G4double weakCons =  0.00799; // M*G^2*cos(th_C)^2/Pi

static const G4double fitConsA = 8.72; // Constant from cross section integration
static const G4double fitConsB = -42.0; // Constant from cross section integration


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

  if ( ZZ != 1)  return 0.;

  else
  {
   	G4double energyE = aPart->GetKineticEnergy()/GeV; // Electron energy

    if (energyE == lastE)  return lastSig*picobarn;// Don't calculate again

    else
    {
      // Calculate maximal cross section at theta = pi
      lastE = energyE;

      // Calculate Cross section
     	lastSig = weakCons * (fitConsA*pow(energyE,2) + fitConsB*pow(energyE,4));

      if(lastSig<0.) lastSig = 0.;

      return lastSig*picobarn;
    }
  }
}

///////////////////////*****************************************************///////////////////////////
