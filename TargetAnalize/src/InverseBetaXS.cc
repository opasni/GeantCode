
#include <iostream>

#include "G4SystemOfUnits.hh"
#include "G4HadTmpUtil.hh"
#include "InverseBetaXS.hh"

// factory
#include "G4CrossSectionFactory.hh"
//
G4_DECLARE_XS_FACTORY(InverseBetaXS);

static const G4double mD = 0.843;
static const G4double mA = 1.060;

//static const G4double fermiG = 1.166

static const G4double gAC = 1.2673;
static const G4double muNe = -1.913;
static const G4double muP = 2.793;

static const G4double weakCons =  0.03;


InverseBetaXS::InverseBetaXS():G4VCrossSectionDataSet(Default_Name()),
M((G4NucleiProperties::GetNuclearMass(1,0)/GeV + G4NucleiProperties::GetNuclearMass(1,1)/GeV)/2), lastE(0), lastSig(0)
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
      G4double Q2 = 4 * energyE*energyE/(1 + (2*energyE/M));
      G4double tau = Q2/(4*M*M);

      G4double GD = std::pow(1+(Q2/(mD*mD)),-2);
      G4double gA = gAC * std::pow(1+(Q2/(mA*mA)),-2);
      // gE = GD * (1 + (muNe * tau)/(1 + 5.6 * tau));
      G4double gM = GD * (muP - muNe);

      G4double f1, f3;
      f1 = gA*gA + tau * (gA*gA + gM*gM);
      f3 = 2 * gA * gM;

     	lastSig = weakCons * (GetParAXS(energyE)*(2*f1 + f3*(M + 2*energyE)/M) - GetParBXS(energyE)*f3);

      if(lastSig<0.) lastSig = 0.;

      return lastSig*picobarn;
    }
  }
}

///////////////////////*****************************************************///////////////////////////

G4double InverseBetaXS::GetParAXS(G4double energyE)
{
  G4double a;
  a = energyE * std::pow(M + energyE,2) * std::pow(M + 2*energyE,-2);
  return a;
}

G4double InverseBetaXS::GetParBXS(G4double energyE)
{
  G4double b;
  b = (1/3) * (energyE/M) * (3*std::pow(M,4) + 12*std::pow(M,3)*energyE + 18*std::pow(M,2)*std::pow(energyE,2)
      + 12*M*std::pow(energyE,3) + 4*std::pow(energyE,3)) * std::pow(M + 2*energyE,-3);
  return b;
}

///////////////////////*****************************************************///////////////////////////
