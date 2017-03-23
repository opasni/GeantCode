
#include <iostream>

#include "G4SystemOfUnits.hh"
#include "G4HadTmpUtil.hh"
#include "InverseBetaXS.hh"

// factory
#include "G4CrossSectionFactory.hh"
//
G4_DECLARE_XS_FACTORY(InverseBetaXS);

static const G4double mD = 843;
static const G4double mA = 1060;

//static const G4double fermiG = 1.166

static const G4double gA = 1.2673;
static const G4double muNe = -1.913;
static const G4double muP = 2.793;

static const G4double weakCons =  0.0150003624;


InverseBetaXS::InverseBetaXS():G4VCrossSectionDataSet(Default_Name()), 
M((G4NucleiProperties::GetNuclearMass(1,0) + G4NucleiProperties::GetNuclearMass(1,1))/2), lastE(0), lastSig(0)
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
   	G4double energyE = aPart->GetKineticEnergy()/MeV; // Electron energy

    if (energyE < 60.) return 0.;

    if (energyE == lastE)  return lastSig*picobarn;// Don't calculate again

    else 
    {
      // Calculate maximal cross section at theta = pi

      lastE = energyE;

      G4double vCMS = 1/(1+M/energyE);

      G4double denNu =  (1 - vCMS)/(1 + vCMS); // Energy of neutrino divided by energy of electron
   
     	G4double Q2 = 4 * energyE*energyE * (1 - vCMS)/(1 + vCMS);
     	G4double tau = Q2/(4 * M*M);
     	G4double GD = std::pow(1+(Q2/(mD*mD)),-2);
     	G4double GA = gA * std::pow(1+(Q2/(mA*mA)),-2);

     	G4double gM = GD * (muP - muNe);

     	G4double form1 = GA*GA + tau * (GA*GA + gM*gM);
     	G4double form3 = 2 * GA * gM;

     	lastSig = weakCons * denNu * ( (2 * form1) + (form3 * energyE * (1 + denNu)/M));

      if(lastSig<0.) lastSig = 0.;

      return lastSig*picobarn;
    }
  }
}