#include "HadCalorimeterSD.hh"
#include "HadCalorimeterHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadCalorimeterSD::HadCalorimeterSD(G4String name, G4int fnofLayers)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1), nofLayers(0)
{
    collectionName.insert("HadCalorimeterColl");
    nofLayers = fnofLayers;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadCalorimeterSD::~HadCalorimeterSD()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadCalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection 
      = new HadCalorimeterHitsCollection(SensitiveDetectorName,collectionName[0]);
    if (fHCID<0)
    { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
    hce->AddHitsCollection(fHCID,fHitsCollection);
    
    // fill calorimeter hits with zero energy deposition
    for (G4int iColumn=0;iColumn<nofLayers;iColumn++)
        for (G4int iRow=0;iRow<nofLayers;iRow++)
        {
            HadCalorimeterHit* hit = new HadCalorimeterHit();
            fHitsCollection->insert(hit);
        }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HadCalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4String name = step->GetTrack()->GetDefinition()->GetParticleName();
    //G4String proces = step->GetTrack()-> GetCreatorProcess()->GetProcessName();
    //if ( name != "neutron") return true;
    //if ( name == "e-") return true;
    //if ( name == "e+") return true;
    //if ( proces == "eBrem") return true;
    //if ( proces == "annihil") return true;

    G4double edep = step->GetTotalEnergyDeposit();
    if (edep==0.) return true;

    G4double timeglob = step->GetTrack()->GetGlobalTime();
    G4ThreeVector postr = step->GetTrack()->GetPosition();
    //G4VProcess process = step->GetTrack()-> GetCreatorProcess();
    //if ( name == "gamma")
    // {G4cout << "Particle name: " << name << " and process name: " << proces << " and time: " << timeglob << G4endl;}
    //G4cout << process.GetProcessName()  << G4endl;

    //if ( timeglob < 60*ns) return true;

    G4TouchableHistory* touchable
      = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());

    G4VPhysicalVolume* cellPhysical = touchable->GetVolume(2);
    G4int rowNo = cellPhysical->GetCopyNo();
    G4VPhysicalVolume* columnPhysical = touchable->GetVolume(3);
    G4int columnNo = cellPhysical->GetCopyNo(); 
    G4VPhysicalVolume* layerPhysical = touchable->GetVolume(1);
    G4int layerNo = layerPhysical->GetCopyNo();

     if (((rowNo==8) ||(rowNo==9) ||(rowNo==10) ||(rowNo==11))
             && ((columnNo==8) ||(columnNo==9) ||(columnNo==10) ||(columnNo==11))) return true;

    G4int hitID = nofLayers*columnNo+rowNo;
    HadCalorimeterHit* hit = (*fHitsCollection)[hitID];
    
    // check if it is first touch
    if (hit->GetColumnID()<0)
    {
        hit->SetColumnID(columnNo);
        hit->SetRowID(layerNo);
        hit->SetTime(timeglob);
        hit->SetPos(postr);
        G4int depth = touchable->GetHistory()->GetDepth();
        G4AffineTransform transform 
          = touchable->GetHistory()->GetTransform(depth-2);
        transform.Invert();
        hit->SetRot(transform.NetRotation());
        hit->SetPos(transform.NetTranslation());
    }
    // add energy deposition
    hit->AddEdep(edep);
    
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
