#include "BVActionInitialization.hh"
#include "BVPrimaryGeneratorAction.hh"
#include "BVRunAction.hh"
#include "BVEventAction.hh"
#include "BVSteppingAction.hh"

BVActionInitialization::BVActionInitialization()
 : G4VUserActionInitialization()
{}

BVActionInitialization::~BVActionInitialization()
{}

void BVActionInitialization::BuildForMaster() const
{
  SetUserAction(new BVRunAction());
}

void BVActionInitialization::Build() const
{
  SetUserAction(new BVPrimaryGeneratorAction());
  SetUserAction(new BVRunAction());

  // 1. Create the EventAction first
  auto eventAction = new BVEventAction();
  SetUserAction(eventAction); // Register it to Geant4

  // 2. Pass it to SteppingAction
  SetUserAction(new BVSteppingAction(eventAction));
}