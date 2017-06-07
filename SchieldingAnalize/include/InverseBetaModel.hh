#ifndef InverseBetaModel_h
#define InverseBetaModel_h 1

#include "G4HadronicInteraction.hh"
#include "G4HadFinalState.hh"

#include "G4VCrossSectionDataSet.hh"
#include "G4DynamicParticle.hh"
#include "G4Element.hh"
#include "G4ParticleTable.hh"
#include "G4NucleiProperties.hh"
#include "G4NistManager.hh"
#include <vector>
#include "Randomize.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include <map>

//class G4ElectroNuclearCrossSection;
class InverseBetaXS;
//class G4CascadeInterface;

class InverseBetaModel : public G4HadronicInteraction
{
public:

  InverseBetaModel();
  ~InverseBetaModel();

  G4HadFinalState* ApplyYourself(const G4HadProjectile& aTrack,
                                   G4Nucleus&);

  virtual void ModelDescription(std::ostream& outFile) const;


  private:

    //G4ElectroNuclearCrossSection* electroXS;
    InverseBetaXS* electroXS;

    G4double CalculateProbability(G4double theta);

    G4HadFinalState* theResult = new G4HadFinalState();

    void CalculateVert(G4double theta, G4ThreeVector momentE, G4double leptonKE);


    const G4double M; // Mean mass of nucleai
    };

#endif
