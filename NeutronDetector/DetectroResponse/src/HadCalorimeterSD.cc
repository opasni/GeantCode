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

    fFirst = 0;

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

    // G4cout << edep << G4endl;

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


    // We calculate the distance the particle traveled through the air, so that we can substitute from total lenght

    G4double removedist;
    G4double detIN = 700*mm;
    if (fFirst==0){
      fFirst=1;
      xMean = vertpos[0];
      yMean = vertpos[1];
      removedist = 0;
    }
    else if (((xMean<-(detIN-1))&&(vertpos[0]<-(detIN-1)))||((xMean>(detIN-1))&&(vertpos[0]>(detIN-1)))) removedist = 0;
    else if (((yMean<-(detIN-1))&&(vertpos[1]<-(detIN-1)))||((yMean>(detIN-1))&&(vertpos[1]>(detIN-1)))) removedist = 0;
    else {
      G4double k = (vertpos[1]-yMean)/(vertpos[0]-xMean);
      G4double n = (yMean*vertpos[0]-xMean*vertpos[1])/(vertpos[0]-xMean);
      G4double p1x=-(2*detIN);
      G4double p1y, p2x,p2y;
      G4double x,y;
      y=k*(-detIN)+n;if ((y>-detIN)&&(y<detIN)) {p1x=-detIN;p1y=y;}
      y=k*(detIN)+n;if ((y>-detIN)&&(y<detIN)) {if (p1x==-(2*detIN)) {p1x=detIN;p1y=y;} else {p2x=detIN;p2y=y;}}
      x=(-detIN-n)/k;if ((x>-detIN)&&(x<detIN)) {if (p1x==-(2*detIN)) {p1x=x;p1y=-detIN;} else {p2x=x;p2y=-detIN;}}
      x=(detIN-n)/k;if ((x>-detIN)&&(x<detIN)) {if (p1x==-(2*detIN)) {p1x=-x;p1y=detIN;} else {p2x=x;p2y=detIN;}}
      if (p1x==-(2*detIN)) removedist = 0;
      else removedist = sqrt(pow(p1x-p2x,2)+pow(p1y-p2y,2));
      // G4double dist = sqrt(pow(xMean-vertpos[0],2)+pow(yMean-vertpos[1],2));
      // if (dist<removedist) G4cout << xMean << ' ' << yMean << ' ' << vertpos[0] << ' ' << vertpos[1] << ' ' << k <<  ' ' << n << ' ' << dist <<  ' ' << removedist << G4endl;
    }
    // G4cout << removedist << G4endl;
    G4ThreeVector posMean = G4ThreeVector(xMean, yMean, removedist);
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
        hit->SetPosMean(posMean);
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
