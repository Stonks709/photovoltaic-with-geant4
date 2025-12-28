#ifndef BVSteppingAction_h
#define BVSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class BVEventAction; // Forward declaration (Tell compiler this class exists)

class BVSteppingAction : public G4UserSteppingAction
{
  public:
    // Update the constructor to accept the EventAction pointer
    BVSteppingAction(BVEventAction* eventAction);
    virtual ~BVSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    // Variable to store the pointer
    BVEventAction* fEventAction;
};

#endif