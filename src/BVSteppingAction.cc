#include "BVSteppingAction.hh"
#include "BVEventAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"

BVSteppingAction::BVSteppingAction(BVEventAction* eventAction)
: G4UserSteppingAction(), fEventAction(eventAction)
{}

BVSteppingAction::~BVSteppingAction() {}

void BVSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();

  // 1. We only care about Optical Photons
  if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) return;

  // 2. Check Volume Name
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4String volName = preStepPoint->GetTouchableHandle()->GetVolume()->GetName();

  // If photon hits Top OR Bottom Panel
  if (volName == "Panel_Top" || volName == "Panel_Bottom") {
      
      // 3. Add to Counter in EventAction
      fEventAction->AddPhoton(track->GetTotalEnergy());

      // 4. Kill the photon (Absorb it) so we don't count it twice
      track->SetTrackStatus(fStopAndKill);
  }
}