#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh" // CRITICAL FOR PHOTONS

#include "BVDetectorConstruction.hh"
#include "BVActionInitialization.hh"

int main(int argc, char** argv)
{
  auto* runManager = G4RunManagerFactory::CreateRunManager();

  // 1. Geometry
  runManager->SetUserInitialization(new BVDetectorConstruction());

  // 2. Physics: Standard EM + OPTICAL PHYSICS
  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->RegisterPhysics(new G4OpticalPhysics());
  runManager->SetUserInitialization(physicsList);

  // 3. User Actions
  runManager->SetUserInitialization(new BVActionInitialization());

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc == 1) {
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute run.mac"); 
    ui->SessionStart();
    delete ui;
  } else {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }

  delete visManager;
  delete runManager;
}