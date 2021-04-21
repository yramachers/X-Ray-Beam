#ifndef SEDetectorConstruction_h
#define SEDetectorConstruction_h 1

#include "G4Cache.hh"
#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4MultiFunctionalDetector;

class SEDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  SEDetectorConstruction();
  ~SEDetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();
  virtual void               ConstructSDandField();

private:
  void DefineMaterials();

  G4VPhysicalVolume* SetupBaseline();

  G4Cache<G4MultiFunctionalDetector*> fSD;
};

#endif
