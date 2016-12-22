#ifndef HadCalorimeterHit_h
#define HadCalorimeterHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;
class G4AttValue;

/// Hadron Calorimeter hit
///
/// It records:
/// - the cell column ID and row ID
/// - the energy deposit 
/// - the cell position and rotation

class HadCalorimeterHit : public G4VHit
{
public:
    HadCalorimeterHit();
    HadCalorimeterHit(G4int iCol,G4int iRow);
    HadCalorimeterHit(const HadCalorimeterHit &right);
    virtual ~HadCalorimeterHit();

    const HadCalorimeterHit& operator=(const HadCalorimeterHit &right);
    int operator==(const HadCalorimeterHit &right) const;
    
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();
    
    void SetColumnID(G4int z) { fColumnID = z; }
    G4int GetColumnID() const { return fColumnID; }

    void SetRowID(G4int z) { fRowID = z; }
    G4int GetRowID() const { return fRowID; }

    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }

    void SetPos(G4ThreeVector xyz) { fPos = xyz; }
    G4ThreeVector GetPos() const { return fPos; }

    void SetTime(G4double t) { fTime = t; }
    G4double GetTime() const { return fTime; }

    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }
    
private:
    G4int fColumnID;
    G4int fRowID;
    G4double fEdep;
    G4ThreeVector fPos;
    G4double fTime;
    G4RotationMatrix fRot;
};

typedef G4THitsCollection<HadCalorimeterHit> HadCalorimeterHitsCollection;

extern G4ThreadLocal G4Allocator<HadCalorimeterHit>* HadCalorimeterHitAllocator;

inline void* HadCalorimeterHit::operator new(size_t)
{
    if (!HadCalorimeterHitAllocator)
        HadCalorimeterHitAllocator = new G4Allocator<HadCalorimeterHit>;
    return (void*)HadCalorimeterHitAllocator->MallocSingle();
}

inline void HadCalorimeterHit::operator delete(void* aHit)
{
    HadCalorimeterHitAllocator->FreeSingle((HadCalorimeterHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
