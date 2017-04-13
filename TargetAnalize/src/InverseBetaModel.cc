#include "InverseBetaModel.hh"

#include "InverseBetaXS.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4CrossSectionDataSetRegistry.hh"

#include "G4HadFinalState.hh"
#include "G4HadronicInteractionRegistry.hh"

#include "G4NeutrinoE.hh"
#include "G4Neutron.hh"

static const G4double mD = 0.843;
static const G4double mA = 1.060;

static const G4double gAC = 1.2673;
static const G4double muNe = -1.913;
static const G4double muP = 2.793;

static const G4double fitCons = 6.5; // Constant from cross section integration

InverseBetaModel::InverseBetaModel()
  : G4HadronicInteraction("InverseBetaModel"),
   M((G4NucleiProperties::GetNuclearMass(1,0)/GeV + G4NucleiProperties::GetNuclearMass(1,1)/GeV)/2)
{
  SetMinEnergy(0.0);
  SetMaxEnergy(1*TeV);
  electroXS =
    (InverseBetaXS*)G4CrossSectionDataSetRegistry::Instance()->
    GetCrossSectionDataSet(InverseBetaXS::Default_Name());
}

InverseBetaModel::~InverseBetaModel()
{}


void InverseBetaModel::ModelDescription(std::ostream& outFile) const
{
  outFile << "InverseBetaModel handles the inverse beta process\n"
          << "of the e- on p to nu_e and n.\n";
}

///////////////////////*****************************************************///////////////////////////

G4HadFinalState*
InverseBetaModel::ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus&)
{

  G4double energyE = aTrack.GetKineticEnergy()/GeV; // Electron energy
  G4ThreeVector momentE = aTrack.Get4Momentum().vect().unit(); // Electron  momentum direction

  G4double theta = std::acos(2*G4UniformRand()-1); // Theta sphericaly

  G4double sigma = CalculateProbability(cos(theta), energyE);  // Calculate sigma(theta)/sigma(pi)
  G4double prob = G4UniformRand();

  // Applying actual distribution

  if (prob < sigma)
  {
    theParticleChange.Clear();
    theParticleChange.SetStatusChange(stopAndKill);

    CalculateVert(theta, momentE, energyE); // Calculate the final state
    return theResult;
  }

  else
  {
   // Set up default particle change (just returns initial state)
    theParticleChange.Clear();
    theParticleChange.SetStatusChange(isAlive);
    energyE = aTrack.GetKineticEnergy();
    theParticleChange.SetEnergyChange(energyE);

    return &theParticleChange;
  }
}

///////////////////////*****************************************************///////////////////////////

G4double InverseBetaModel::CalculateProbability(G4double theta, G4double energyE)
{

  // Calculate Probability

  G4double enerL = energyE/(1 + (energyE*(1-theta)/M));
  G4double enerN = energyE + M - enerL;

  G4double Q2 = 2 * M * (energyE - enerL);
  G4double tau = Q2/(4*M*M);

  G4double GD = std::pow(1+(Q2/(mD*mD)),-2);
  G4double gA = gAC * std::pow(1+(Q2/(mA*mA)),-2);
  G4double gE = GD * (1 + (muNe * tau)/(1 + 5.6 * tau));
  G4double gM = GD * (muP - muNe);

  G4double f1, f2, f3;
  f1 = gA*gA + tau * (gA*gA + gM*gM);
  f2 = gA*gA + (gE*gE + tau * gM*gM)/(1 + tau);
  f3 = 2 * gA * gM;

  G4double enerpart = (enerL*enerL*enerN)/(energyE*energyE*M)*(1/fitCons);
  G4double anglepart = f2 + (2*f1 - f2 + ((energyE + enerL)/M)*f3)*(1-theta)/2;

  return (enerpart*anglepart);

}

///////////////////////*****************************************************///////////////////////////

void InverseBetaModel::CalculateVert(G4double theta, G4ThreeVector momentE, G4double energyE)
{

  G4double phi = twopi*G4UniformRand();

  G4double enerL, enerN, mLep, mN, thN;
  enerL = energyE/(1 + (2*energyE*std::pow(std::sin(theta/2),2)/M));
  enerN = energyE + M - enerL;
  mLep = enerL;
  mN = std::sqrt(enerN*enerN - M*M);
  thN = std::asin((mLep/mN)*std::sin(theta));

  G4ThreeVector momL;
    momL.setX(mLep * std::sin(theta) * std::sin(phi));
    momL.setY(mLep * std::sin(theta) * std::cos(phi));
    momL.setZ(mLep * std::cos(theta));

  momL.rotateUz(momentE); // Rotation according to initial electron momentum

  G4double KinEnL = mLep*GeV; // Kinetic energy of neutron

  G4ThreeVector momN;
    momN.setX(mN * std::sin(thN) * std::sin(phi));
    momN.setY(mN * std::sin(thN) * std::cos(phi));
    momN.setZ(mN * std::cos(thN));

  momN.rotateUz(momentE); // Rotation according to initial electron momentum

  G4double KinEnN = (enerN - M)*GeV; // Kinetic energy of neutron

  // Particles definitions

  G4DynamicParticle* theNeutrino = new G4DynamicParticle;
    theNeutrino->SetDefinition(G4NeutrinoE::Definition());
    theNeutrino->SetMomentum( momL );
    theNeutrino->SetKineticEnergy( KinEnL );

  G4DynamicParticle*  theNeutron = new G4DynamicParticle;
    theNeutron->SetDefinition( G4Neutron::Definition() );
    theNeutron->SetMomentum( momN );
    theNeutron->SetKineticEnergy( KinEnN );

  theResult->AddSecondary( theNeutrino );
  theResult->AddSecondary( theNeutron );

}

///////////////////////*****************************************************///////////////////////////
