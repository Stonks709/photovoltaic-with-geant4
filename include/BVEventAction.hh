#include "G4UserEventAction.hh"
#include "globals.hh"

class BVEventAction : public G4UserEventAction
{
  public:
    BVEventAction();
    virtual ~BVEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    // Function for SteppingAction to call
    void AddPhoton(G4double energy) { 
        fPhotonCount++; 
        fTotalEnergy += energy;
    }

  private:
    G4int fPhotonCount;
    G4double fTotalEnergy;
};