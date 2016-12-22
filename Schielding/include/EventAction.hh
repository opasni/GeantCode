#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>

/// Event action class
///

class DetectorConstruction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(G4int fnofLayers);
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* event);
    virtual void    EndOfEventAction(const G4Event* event);

    void AddParticle(G4String name, G4double posX, G4double posY, G4double timeglob, G4double energy, G4String proces);
    
  private:

    //G4double TransformToRadius(G4int row, G4int column, G4int mid);

    G4int nofLayers;

    std::vector<G4String> fname;
    std::vector<G4double> fposX;
    std::vector<G4double> fposY;
    std::vector<G4double> ftime;
    std::vector<G4double> fenergy;
    std::vector<G4String> fprocess;

};

// inline functions

inline void EventAction::AddParticle(G4String name, G4double posX, G4double posY, 
                                     G4double timeglob, G4double energy, G4String proces) 
{
  fname.push_back(name);
  fposX.push_back(posX);
  fposY.push_back(posY);
  ftime.push_back(timeglob);
  fenergy.push_back(energy);
  fprocess.push_back(proces);
}
    
#endif

    
