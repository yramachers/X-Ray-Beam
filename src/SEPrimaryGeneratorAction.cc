// us
#include "SEPrimaryGeneratorAction.hh"

// geant
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

SEPrimaryGeneratorAction::SEPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
, fParticleGun(nullptr)
{
  generator.seed(rd());  // set a random seed

  G4int nofParticles = 1;
  fParticleGun       = new G4ParticleGun(nofParticles);

  auto particleTable = G4ParticleTable::GetParticleTable();

  // default particle kinematics
  fParticleGun->SetParticleDefinition(particleTable->FindParticle("gamma"));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.)); // down z-axis
  fParticleGun->SetParticleEnergy(15.0*keV);
}

SEPrimaryGeneratorAction::~SEPrimaryGeneratorAction()
{
  delete fParticleGun;
}

void SEPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume 
  // from G4LogicalVolumeStore: assumes name is World_log!
  //
  auto worldLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World_log");
  G4Box* worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  G4double worldZHalfLength = worldBox->GetZHalfLength();

  std::uniform_real_distribution<double> rndm(0.0, 1.0);  // random uniform number 
  std::normal_distribution<double> gauss(0.0, 8.7e-3);  // Gaussian distribution [mm] 

  G4double rad    = gauss(generator) * CLHEP::mm;
  G4double phi    = CLHEP::twopi * rndm(generator);

  G4double xpos = rad * std::sin(phi);
  G4double ypos = rad * std::cos(phi);

  fParticleGun->SetParticlePosition(G4ThreeVector(xpos, ypos, worldZHalfLength - 1.*mm));

  fParticleGun->GeneratePrimaryVertex(event);
}

