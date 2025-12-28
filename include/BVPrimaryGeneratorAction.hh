#ifndef BVPrimaryGeneratorAction_h
#define BVPrimaryGeneratorAction_h 1
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
class BVPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    BVPrimaryGeneratorAction();
    virtual ~BVPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);
  private:
    G4ParticleGun* fParticleGun;
};
#endif