#ifndef SEPrimaryGeneratorAction_h
#define SEPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

#include <random>

class G4ParticleGun;
class G4Event;


/// Primary generator
///
/// A single particle is generated.

class SEPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  SEPrimaryGeneratorAction();
  virtual ~SEPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

private:

  G4ParticleGun*      fParticleGun;
  std::random_device  rd;
  std::ranlux24       generator;

};

#endif
