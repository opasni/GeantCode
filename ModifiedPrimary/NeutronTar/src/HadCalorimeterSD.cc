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

HadCalorimeterSD::HadCalorimeterSD(G4String name, G4int fnofLayers, G4int fnofLayersZ)
: G4VSensitiveDetector(name), fHitsCollection(0), fHCID(-1), nofLayers(0), nofLayersZ(0)
{
    collectionName.insert("HadCalorimeterColl");
    nofLayers = fnofLayers;
    nofLayersZ = fnofLayersZ;
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
    {
      for (G4int iRow=0;iRow<nofLayers;iRow++)
      {
        for (G4int iLay=0;iLay<nofLayersZ;iLay++)
        {
            HadCalorimeterHit* hit = new HadCalorimeterHit();
            fHitsCollection->insert(hit);
        }
      }
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HadCalorimeterSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep==0.) return true;

    G4double timeglob = step->GetTrack()->GetGlobalTime();
    G4ThreeVector vertpos = step->GetTrack()->GetPosition();
    // G4ThreeVector vertpos = step->GetTrack()->GetVertexPosition();

    // G4String process = step->GetTrack()-> GetCreatorProcess()->GetProcessName();
    // G4String name = step->GetTrack()->GetDefinition()->GetParticleName();

    G4int parentNo = step->GetTrack()->GetParentID();

    //if ( timeglob < 60*ns) return true;

    G4TouchableHistory* touchable
      = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());

    G4VPhysicalVolume* layerPhysical = touchable->GetVolume(1);
    G4int layerNo = layerPhysical->GetCopyNo();
    G4VPhysicalVolume* cellPhysical = touchable->GetVolume(2);
    G4int rowNo = cellPhysical->GetCopyNo();
    G4VPhysicalVolume* columnPhysical = touchable->GetVolume(3);
    G4int columnNo = columnPhysical->GetCopyNo();

        //  if (((rowNo==8) ||(rowNo==9) ||(rowNo==10) ||(rowNo==11))
        //          && ((columnNo==8) ||(columnNo==9) ||(columnNo==10) ||(columnNo==11))) G4cout << vertpos << G4endl;


    // G4int hitID = nofLayers*columnNo+rowNo;
    G4int hitID = nofLayersZ*(nofLayers*columnNo + rowNo) + layerNo;
    HadCalorimeterHit* hit = (*fHitsCollection)[hitID];

    // check if it is first touch
    if (hit->GetColumnID()<0)
    {
        hit->SetColumnID(columnNo);
        hit->SetRowID(rowNo);
        hit->SetLayerID(layerNo);
        hit->SetParentID(parentNo);
        hit->SetTime(timeglob);
        hit->SetPos(vertpos);
        // hit->SetProcess(process);
        // hit->SetName(name);
        G4int depth = touchable->GetHistory()->GetDepth();
        G4AffineTransform transform
          = touchable->GetHistory()->GetTransform(depth-2);
        transform.Invert();
        hit->SetRot(transform.NetRotation());
        // hit->SetPos(transform.NetTranslation());
    }
    // add energy deposition
    hit->AddEdep(edep);

    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
