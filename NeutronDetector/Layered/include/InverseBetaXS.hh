#ifndef InverseBetaXS_h
#define InverseBetaXS_h 1

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

class InverseBetaXS : public G4VCrossSectionDataSet
{
public:

  InverseBetaXS();
  virtual ~InverseBetaXS();

  static const char* Default_Name() {return "InverseBetaXS";}

  virtual void CrossSectionDescription(std::ostream&) const;

  virtual G4bool IsElementApplicable(const G4DynamicParticle*, G4int Z,
                                     const G4Material*);
  virtual G4double GetElementCrossSection(const G4DynamicParticle*, G4int Z,
                                    const G4Material*);
  // Body
private:

    G4NistManager* nistmngr;

    // Cache values for XS
    G4double lastE ; // Last used energy value
    G4double lastSig; //Last used XS value

    };

#endif
