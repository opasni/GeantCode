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

static const G4double fitConsA = 0.162; // Constant from cross section integration
static const G4double fitConsB = 0.069; // Constant from cross section integration
static const G4double fitConsC = 0.021; // Constant from cross section integration

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

  G4double theta = 0;
  G4double sigma = 0;
  G4double prob = 1;
  // Applying actual distribution
  while (1)
  {
    theta = std::acos(2*G4UniformRand()-1); // Theta sphericaly
    sigma = CalculateProbability(theta);  // Calculate sigma(theta)/sigma(pi)
    prob = G4UniformRand()*0.54;
    if (prob < sigma)
    {
      theParticleChange.Clear();
      theParticleChange.SetStatusChange(stopAndKill);
      CalculateVert(theta, momentE, energyE); // Calculate the final state
      return theResult;
      break;
    }
  }
}

///////////////////////*****************************************************///////////////////////////

G4double InverseBetaModel::CalculateProbability(G4double theta)
{
  // Calculate Probability
  G4double prob = fitConsA*theta + fitConsB*pow(theta,2) + fitConsC*pow(theta,3);
  return prob;

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
