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

static const G4double massM1 = 4.693; // Constant from cross section integration
static const G4double massM2 = 40.77; // Constant from cross section integration
static const G4double massM3 = -28.72; // Constant from cross section integration

static const G4double Mv2 = 0.711;          // Kvadrat dipolne mase
static const G4double Ma2 = 1.053;
static const G4double gA = -1.267;
static const G4double aPar = 0.942;
static const G4double bPar = 4.61;
static const G4double mup = 2.793;
static const G4double mun = -1.913;

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

  G4double energyE = aTrack.GetKineticEnergy()/GeV;             // Energija elektrona
  G4ThreeVector momentE = aTrack.Get4Momentum().vect().unit();  // Smer gibalne količine e-

  while (1){                                                    // Ponavljamo dokler ne uspe
    G4double theta = std::acos(2*G4UniformRand()-1);            // Žreb kota sipanja (nevtrino)
    G4double sigma = CalculateProbability(theta, energyE);      // Izračun  verjetnosti P
    G4double prob = G4UniformRand();                            // Naključna vrednost

  // Primerjamo sigma in prob ter sprejmemo proces v primeru da vrednost prob manjša
    if (prob < sigma)
    {
      theParticleChange.Clear();
      theParticleChange.SetStatusChange(stopAndKill);           // Izbrišemo elektron

      CalculateVert(theta, momentE, energyE);                   // Izračunamo končno stanje reakcije
      return theResult;
    }
  }
}
  // V primeru da je prob večji, elektron nadaljuje pot brez sipanja
  /*else
  {
    theParticleChange.Clear();
    theParticleChange.SetStatusChange(isAlive);
    energyE = aTrack.GetKineticEnergy();
    theParticleChange.SetEnergyChange(energyE);

    return &theParticleChange;
  }
}*/

///////////////////////*****************************************************///////////////////////////

G4double InverseBetaModel::CalculateProbability(G4double theta, G4double energyE)
{
  // Izračun kinematičnih spremenljivk
  G4double enerL = energyE/(1 + (2*energyE*std::pow(std::sin(theta/2),2)/M));   // Energija nevtrina
  G4double Q2 = 2 * M * (energyE - enerL);
  G4double tau = (energyE - enerL) / (2 * M);
  G4double suM = 2 * (energyE + enerL) / M;
  G4double Q2M = 2 * (energyE - enerL) /  M;

  // Izračun strukturnih faktorjev
  G4double GD, GA, GEp, GMp, GEn, GMn, gE, gM;

  GD = 1/pow(1+Q2/Mv2,2);                             GA = gA/pow(1+Q2/Ma2,2);
  GEp = GD;                                           GMp = mup*GD;
  GEn = -mun * aPar * tau * GD / (1 + bPar*tau);      GMn = mun * GD;
  gE = GEp - GEn;
  gM = GMp - GMn;

  // Izračun strukturnih funkcij
  G4double F1 = (gE + tau*gM) / (1 + tau);    // Povprečene vrednosti FF
  G4double F2 = (gM - gE) / (1 + tau);
  G4double FA = GA;

  G4double Af = Q2M * ((1+tau)*FA*FA - (1-tau)*F1*F1 + (1-tau)*tau*F2*F2 + 4*tau*F1*F2);
  G4double Bf = Q2M * FA * (F1 + F2);
  G4double Cf = (FA*FA + F1*F1 + tau*F2*F2) / 4;

  // Izračun energijskega in kotnega dela, pri čemer delimo diferencialni presek s celotnim
  G4double enerpart = (2 * M*M * enerL*enerL * pow(0.5*M + energyE,3)) /
                      (pow(energyE,4)*(massM1 + massM2*energyE + massM3*pow(energyE,2)));

  G4double anglepart = Af + suM*Bf + suM*suM*Cf;

  return (enerpart*anglepart);
}

///////////////////////*****************************************************///////////////////////////

void InverseBetaModel::CalculateVert(G4double theta, G4ThreeVector momentE, G4double energyE)
{
  G4double phi = twopi*G4UniformRand();                                 // Naključni polarni kot
  G4double enerL, enerN, mLep, mN, thN;
  enerL = energyE/(1 + (2*energyE*std::pow(std::sin(theta/2),2)/M));    // Energija nevtrina
  enerN = energyE + M - enerL;                                          // Energija nevtrona
  mLep = enerL;                                                         // Gibalna količina nevtrina
  mN = std::sqrt(enerN*enerN - M*M);                                    // Gibalna količina nevtrona
  thN = std::asin((mLep/mN)*std::sin(theta));                           // Sipalni kot nevtrona

  G4ThreeVector momL;
    momL.setX(mLep * std::sin(theta) * std::sin(phi));
    momL.setY(mLep * std::sin(theta) * std::cos(phi));
    momL.setZ(mLep * std::cos(theta));
  momL.rotateUz(momentE);                                                // Rotacija glede na začetno smer e-
  G4double KinEnL = mLep*GeV;                                            // Kinetična energija nevtrina

  G4ThreeVector momN;
    momN.setX(mN * std::sin(thN) * std::sin(phi));
    momN.setY(mN * std::sin(thN) * std::cos(phi));
    momN.setZ(mN * std::cos(thN));
  momN.rotateUz(momentE);                                                // Rotacija glede na začetno smer e-
  G4double KinEnN = (enerN - M)*GeV;                                     // Kinetična energija nevtrona

  // Definicija sekundarnih delcev
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
