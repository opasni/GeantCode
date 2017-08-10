#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <iostream>
#include <fstream>
#include <vector>


class G4Run;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    //void PrintToFile(std::vector<G4ThreeVector> fPosition);

  private:
  	//std::ofstream fOutputDataPositions;
};
/*
inline void RunAction::PrintToFile(std::vector<G4ThreeVector> fPosition){
	int n = fPosition.size();
	for (int i=1; i<=n; i++){
		G4ThreeVector pos = fPosition[i];
		//G4double x = int((pos->GetX())*10000)/10000;
		//G4double y = int((pos->GetY())*10000)/10000;
		//G4double z = int((pos->GetZ())*10000)/10000;
		G4double x = pos[0]; if (x < 0.0001) { x = 0.;}
		G4double y = pos[1]; if (y < 0.0001) { y = 0.;}
		G4double z = pos[2]; if (z < 0.0001) { z = 0.;}
		if (x > 1000000 || y > 1000000 ||z > 1000000) { }
		else if	(x < 0.0001 && y < 0.0001 && z < 0.0001) { }
		else {
			//G4cout << x << G4endl;
			//G4cout << y << G4endl;
			//G4cout << z << G4endl;
			G4cout << '{' << x << ',' << y << ',' << z << '}' << G4endl;
		}
	}
} */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

