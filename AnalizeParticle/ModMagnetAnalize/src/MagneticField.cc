#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField()
: G4MagneticField(), fMessenger(0), fBy(1*tesla)
{
    // define commands for this class
    DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::~MagneticField()
{
    delete fMessenger;
}

void MagneticField::GetFieldValue(const G4double Point[4],double *bField) const
{
    // G4double norm = sqrt(Point[0]*Point[0]+Point[1]*Point[1]);
    bField[0] = 0.;
    // bField[0] = fBy*Point[0]/norm;
    bField[1] = fBy;
    // bField[1] = fBy*Point[1]/norm;
    bField[2] = 0.;
    // bField[2] = fBy;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MagneticField::DefineCommands()
{
    // Define //field command directory using generic messenger class
    fMessenger = new G4GenericMessenger(this,
                                        "//field/",
                                        "Field control");

    // fieldValue command
    G4GenericMessenger::Command& valueCmd
      = fMessenger->DeclareMethodWithUnit("value","tesla",
                                  &MagneticField::SetField,
                                  "Set field strength.");
    valueCmd.SetParameterName("field", true);
    valueCmd.SetDefaultValue("1.");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
