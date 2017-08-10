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

static const G4double mD = 843;
static const G4double mA = 1060;

//static const G4double fermiG = 1.166

static const G4double gA = 1.2673;
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

    
  // Build Bertini model
  //bert = new G4CascadeInterface();    
}

InverseBetaModel::~InverseBetaModel()
{}    


void InverseBetaModel::ModelDescription(std::ostream& outFile) const 
{
  outFile << "InverseBetaModel handles the inverse beta process\n"
          << "of the e- on p to nu_e and n.\n";
}

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

G4double InverseBetaModel::CalculateProbability(G4double theta, G4double energyE)
{

   // CMS kinematics

  G4double vCMS, gammaCMS, momentumCMS;

  vCMS = 1/(1+M/energyE);
  gammaCMS = std::sqrt(1/(1-vCMS*vCMS));
  momentumCMS = energyE * gammaCMS*(1 - vCMS);

  // Final energy of neutron (NE) and neutrino (NU) for theta and pi in CMS

  G4double enerNu, enerNuMAX; 

  enerNu = momentumCMS * gammaCMS * (1 + vCMS * std::cos(theta));

  enerNuMAX = momentumCMS * gammaCMS * (1 - vCMS);

  // Sine and sine squared of scattering angle of neutrino

  G4double sinTh, sinTh2;

  sinTh = std::sin(0.5 * std::atan2(std::sin(theta), (gammaCMS * (vCMS + std::cos(theta)))));
  sinTh2 = sinTh*sinTh;

  // Calculation of FF for theta and pi (MAX)

  G4double Q2, Q2MAX, tau, tauMAX, GD, GDMAX, GA, GAMAX, gE, gM, gMMAX;

  Q2 =  4 * energyE * sinTh2 * (1 - vCMS)/(1 + vCMS);
  Q2MAX =  4 * energyE * (1 - vCMS)/(1 + vCMS);

  tau = Q2/(4 * M*M);
  tauMAX = Q2MAX/(4 * M*M);


  GD = std::pow(1+(Q2/(mD*mD)),-2); // Dipole FF
  GDMAX = std::pow(1+(Q2MAX/(mD*mD)),-2);
  GA = gA * std::pow(1+(Q2/(mA*mA)),-2); // Axial FF
  GAMAX = gA * std::pow(1+(Q2MAX/(mA*mA)),-2);

  gE = GD * (1 + (muNe * tau)/(1 + 5.6 * tau)); // Charge FF gE = gEMAX
  gM = GD * (muP - muNe);
  gMMAX = GDMAX * (muP - muNe);

  // Final FF

  G4double form1, form1MAX, form2, form3, form3MAX;

  form1 = GA*GA + tau * (GA*GA + gM*gM);
  form1MAX = GAMAX*GAMAX + tauMAX * (GAMAX*GAMAX + gMMAX*gMMAX);

  form2 = GA*GA + (gE*gE + tau * gM*gM)/(1 + tau);
  
  form3 = 2 * GA * gM;
  form3MAX = 2 * GAMAX * gMMAX;

  // Probability

  G4double sigma, sigmapi; 

  sigma = enerNu*(form2 + (2* form1 - form2 + form3 * (energyE + enerNu)/M) * sinTh2);
  sigmapi = enerNuMAX * (2* form1MAX + form3MAX * (energyE + enerNuMAX)/M);

  return (sigma/sigmapi);

}

void InverseBetaModel::CalculateVert(G4double theta, G4ThreeVector momentE, G4double energyE)
{

    G4double phi = twopi*G4UniformRand();

   // CMS kinematics

  G4double vCMS, gammaCMS, momentumCMS, betaNeutron;

  vCMS = 1/(1+M/energyE);
  gammaCMS = std::sqrt(1/(1-vCMS*vCMS));
  momentumCMS = energyE * gammaCMS*(1 - vCMS); 
  betaNeutron = momentumCMS/(std::sqrt(M*M+momentumCMS*momentumCMS));

  // Final momentum of neutrino (Nu) and neutron (Ne)

  G4ThreeVector momNu;
    momNu.setX(momentumCMS * std::sin(theta) * std::sin(phi)); 
    momNu.setY(momentumCMS * std::sin(theta) * std::cos(phi));
    momNu.setZ(momentumCMS * gammaCMS * (vCMS + std::cos(theta)));

  momNu.rotateUz(momentE); // Rotation according to initial electron momentum

  G4double kinEnNu = momentumCMS * gammaCMS * (1 + vCMS*std::cos(theta)); // Kinetic energy of neutrino

  G4double nuXp = -1; G4double nuYp = -1;
  G4ThreeVector momNe;
    momNe.setX(nuXp * momentumCMS * std::sin(theta) * std::sin(phi)); 
    momNe.setY(nuYp * momentumCMS * std::sin(theta) * std::cos(phi));
    momNe.setZ(momentumCMS * gammaCMS * (vCMS/betaNeutron - std::cos(theta))); 

  momNe.rotateUz(momentE); // Rotation according to initial electron momentum

  G4double KinEnNe = momentumCMS * vCMS * gammaCMS * (1 - std::cos(theta)); // Kinetic energy of neutron
  //G4cout << "Kineticna energija neutrona: " << KinEnNe/MeV << G4endl;
  // Particles definitions

  G4DynamicParticle* theNeutrino = new G4DynamicParticle;
    theNeutrino->SetDefinition(G4NeutrinoE::Definition());
    theNeutrino->SetMomentum( momNu );
    theNeutrino->SetKineticEnergy( kinEnNu );

  G4DynamicParticle*  theNeutron = new G4DynamicParticle;
    theNeutron->SetDefinition( G4Neutron::Definition() );
    theNeutron->SetMomentum( momNe );
    theNeutron->SetKineticEnergy( KinEnNe );
  
  theResult->AddSecondary( theNeutrino );
  theResult->AddSecondary( theNeutron );
    
}