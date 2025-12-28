#include "BVPrimaryGeneratorAction.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

BVPrimaryGeneratorAction::BVPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction()
{
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  // Default Particle: Electron
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("e-");
  fParticleGun->SetParticleDefinition(particle);
  
  // Average Tritium Energy
  fParticleGun->SetParticleEnergy(5.7*keV);
}

BVPrimaryGeneratorAction::~BVPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void BVPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Random Position inside Tritium Layer (3.5cm x 1.5cm x 0.888mm)
  G4double x0 = 3.5*cm * (G4UniformRand() - 0.5);
  G4double y0 = 1.5*cm * (G4UniformRand() - 0.5);
  G4double z0 = 0.888*mm * (G4UniformRand() - 0.5);

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));

  // Random Direction (Isotropic)
  G4double cosTheta = 2*G4UniformRand() - 1;
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double phi = 360.*deg * G4UniformRand();
  
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(sinTheta*std::cos(phi), sinTheta*std::sin(phi), cosTheta));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}