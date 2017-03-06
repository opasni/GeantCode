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

InverseBetaModel::InverseBetaModel()
  : G4HadronicInteraction("InverseBetaModel"),
   M((G4NucleiProperties::GetNuclearMass(1,0) + G4NucleiProperties::GetNuclearMass(1,1))/2)
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
InverseBetaModel::ApplyYourself(const G4HadProjectile& aTrack, G4Nucleus& targetNucleus)
{

  G4double energyE = aTrack.GetKineticEnergy(); // Electron energy
  G4ThreeVector momentE = aTrack.Get4Momentum().vect().unit(); // Electron  momentum direction

  G4double theta = std::acos(2*G4UniformRand()-1); // Theta sphericaly

  G4double sigma = CalculateProbability(theta, energyE);  // Calculate sigma(theta)/sigma(pi)
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
  G4double a, b;
  a = GetParA(energyE);
  b = GetParB(energyE);

  // Calculate Maximal Cross

  G4double Q2 = 4 * energyE*energyE/(1 + (2*energyE/M));
  G4double tau = Q2/(4*M*M);

  G4double GD = std::pow(1+(Q2/(mD*mD)),-2);
  G4double gA = gAC * std::pow(1+(Q2/(mA*mA)),-2);
  G4double gM = GD * (muP - muNe);

  G4double f1M, f3M;
  f1M = gA*gA + tau * (gA*gA + gM*gM);
  f3M = 2 * gA * gM;

  G4double sigmaPI = (a*(2*f1M + f3M*(M + 2*energyE)/M) - b*f3M);

  // Calculate Actual Cross

  G4double neutE = energyE/(1 + (2*energyE*std::pow(std::sin(theta/2),2)/M));
  Q2 = 4 * energyE*neutE*std::pow(std::sin(theta/2),2);
  tau = Q2/(4*M*M);

  GD = std::pow(1+(Q2/(mD*mD)),-2);
  gA = gAC * std::pow(1+(Q2/(mA*mA)),-2);
  G4double gE = GD * (1 + (muNe * tau)/(1 + 5.6 * tau));
  gM = GD * (muP - muNe);

  G4double f1, f2, f3;
  f1 = gA*gA + tau * (gA*gA + gM*gM);
  f2 = gA*gA + (gE*gE + tau * gM*gM)/(1 + tau);
  f3 = 2 * gA * gM;

  G4double A = f2 + (2*f1 - f2 + f3*(M + 2*energyE)/M)*std::pow(std::sin(theta/2),2);
  G4double B = f3*std::pow(std::sin(theta/2),2);

  G4double sigma = (a*A - b*B);

  return (sigma/sigmaPI);

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

  G4double KinEnL = mLep; // Kinetic energy of neutron

  G4ThreeVector momN;
    momN.setX(mN * std::sin(thN) * std::sin(phi));
    momN.setY(mN * std::sin(thN) * std::cos(phi));
    momN.setZ(mN * std::cos(thN));

  momN.rotateUz(momentE); // Rotation according to initial electron momentum

  G4double KinEnN = enerN - M; // Kinetic energy of neutron
  //G4cout << "Kineticna energija neutrona: " << KinEnNe/MeV << G4endl;
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

G4double InverseBetaModel::GetParA(G4double energyE)
{
  G4double a;
  a = energyE * std::pow(M + energyE,2) * std::pow(M + 2*energyE,-2);
  return a;
}

G4double InverseBetaModel::GetParB(G4double energyE)
{
  G4double b;
  b = (1/3) * (energyE/M) * (3*std::pow(M,4) + 12*std::pow(M,3)*energyE + 18*std::pow(M,2)*std::pow(energyE,2)
      + 12*M*std::pow(energyE,3) + 4*std::pow(energyE,3)) * std::pow(M + 2*energyE,-3);
  return b;
}
