#ifndef BVDetectorConstruction_h
#define BVDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

class BVDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    BVDetectorConstruction();
    virtual ~BVDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  private:
    G4LogicalVolume* fScoringVolume;
};
#endif