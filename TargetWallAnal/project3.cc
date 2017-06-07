#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIcommand.hh"

#include "MyPhysicsList.hh"
#include "QGSP_BERT.hh"

#include "Randomize.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
namespace {
  void PrintUsage() {
    G4cerr << " Usage: " << G4endl;
    G4cerr << " project3 [-m macro ] [-u UIsession] [-t nThreads]" << G4endl;
    G4cerr << "   note: -t option is available only for multi-threaded mode."
           << G4endl;
  }
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  // Choose the Random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
#else
  //my Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
  G4RunManager* runManager = new G4RunManager;
#endif
  G4String targetwallmat, bremmstralung;
  G4cout << "Do you want target wall?" << G4endl;
  // G4cin >> targetwallmat;
  targetwallmat = "yes";
  G4cout << "Target cell Material set to: " << targetwallmat << G4endl;
  G4cout << "Bremmstralung on?" << G4endl;
  // G4cin >> bremmstralung;
  bremmstralung = "yes";
  G4cout << "Bremmstralung set to: " << bremmstralung << G4endl;
  // Set mandatory initialization classes
  DetectorConstruction* detConstruction = new DetectorConstruction(targetwallmat);
  // Set mandatory initialization classes
  runManager->SetUserInitialization(detConstruction);

  // G4PhysListFactory factory;
  G4VModularPhysicsList* phys = 0;


  phys = new MyPhysicsList(bremmstralung);
  //phys = new QGSP_BERT();

  phys->SetVerboseLevel(1);
  runManager->SetUserInitialization(phys);
  //PhysicsList* physicsList1 = new PhysicsList(detConstruction); runManager->SetUserInitialization(physicsList1);
  //runManager-> SetUserInitialization(new OpNovicePhysicsList());



  ActionInitialization* actionInitialization
     = new ActionInitialization(detConstruction);
  runManager->SetUserInitialization(actionInitialization);

  // Initialize visualization
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  if ( argc==1 ) {
    // interactive mode : define UI session
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    G4UIExecutive* ui = 0;
    ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete visManager;
    delete ui;
  }
  else  {
    // batch mode
    G4String command = "/control/execute ";
    G4String macro = argv[1];
    UImanager->ApplyCommand(command+macro);
  }

  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
