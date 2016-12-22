#include "HadCalorimeterHit.hh"
#include "DetectorConstruction.hh"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<HadCalorimeterHit>* HadCalorimeterHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadCalorimeterHit::HadCalorimeterHit()\
: G4VHit(), fColumnID(-1), fRowID(-1), fEdep(0.), fPos(0), fTime(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadCalorimeterHit::HadCalorimeterHit(G4int iCol,G4int iRow)
: G4VHit(), fColumnID(iCol), fRowID(iRow), fEdep(0.), fPos(0), fTime(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadCalorimeterHit::~HadCalorimeterHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadCalorimeterHit::HadCalorimeterHit(const HadCalorimeterHit &right)
: G4VHit() {
    fColumnID = right.fColumnID;
    fRowID = right.fRowID;
    fEdep = right.fEdep;
    fPos = right.fPos;
    fTime = right.fTime;
    fRot = right.fRot;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const HadCalorimeterHit& HadCalorimeterHit::operator=(const
                                    HadCalorimeterHit &right)
{
    fColumnID = right.fColumnID;
    fRowID = right.fRowID;
    fEdep = right.fEdep;
    fPos = right.fPos;
    fTime = right.fTime;
    fRot = right.fRot;
    return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int HadCalorimeterHit::operator==(const HadCalorimeterHit &right) const
{
    return (fColumnID==right.fColumnID&&fRowID==right.fRowID);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadCalorimeterHit::Draw()
{/*
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if (pVVisManager&&(fEdep>0.))
    {
        // Draw a calorimeter cell with depth propotional to the energy deposition
        G4Transform3D trans(fRot.inverse(),fPos);
        G4VisAttributes attribs;
        G4Colour colour(1.,0.,0.);
        attribs.SetColour(colour);
        attribs.SetForceSolid(true);

        //G4double detectXY = 0.5*m;
        //G4int nofLayers = 20;
        //G4Box box("dummy", detectXY/nofLayers, detectXY/nofLayers, 0.5*m*fEdep/(0.1*GeV));
        //pVVisManager->Draw(box,attribs,trans);
    }*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* HadCalorimeterHit::GetAttDefs() const
{
    G4bool isNew;
    std::map<G4String,G4AttDef>* store
    = G4AttDefStore::GetInstance("HadCalorimeterHit",isNew);

    if (isNew) {
        (*store)["HitType"] 
          = G4AttDef("HitType","Hit Type","Physics","","G4String");
        
        (*store)["Column"] 
          = G4AttDef("Column","Column ID","Physics","","G4int");
        
        (*store)["Row"] 
          = G4AttDef("Row","Row ID","Physics","","G4int");
        
        (*store)["Energy"] 
          = G4AttDef("Energy","Energy Deposited","Physics","G4BestUnit",
                     "G4double");
        
        (*store)["Pos"] 
          = G4AttDef("Pos", "Position", "Physics","G4BestUnit",
                     "G4ThreeVector");
        
        (*store)["Time"] 
          = G4AttDef("Time","Time of Hit","Physics","G4BestUnit",
                     "G4double");
    }
    return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* HadCalorimeterHit::CreateAttValues() const
{
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
    
    values
      ->push_back(G4AttValue("HitType","HadCalorimeterHit",""));
    values
      ->push_back(G4AttValue("Column",G4UIcommand::ConvertToString(fColumnID),
                             ""));
    values
      ->push_back(G4AttValue("Row",G4UIcommand::ConvertToString(fRowID),""));
    values
      ->push_back(G4AttValue("Energy",G4BestUnit(fEdep,"Energy"),""));
    values
      ->push_back(G4AttValue("Pos",G4BestUnit(fPos,"Length"),""));
    values
      ->push_back(G4AttValue("Time",G4BestUnit(fTime,"Time"),""));
    
    return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadCalorimeterHit::Print()
{
    G4cout << "  Cell[" << fRowID << ", " << fColumnID << "] "
    << fEdep/MeV << " (MeV) " << fPos << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
