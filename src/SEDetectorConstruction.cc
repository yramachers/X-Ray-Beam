#include "SEDetectorConstruction.hh"

#include "G4RunManager.hh"

#include "G4Box.hh"
#include "G4GeometryManager.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4PVPlacement.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"
#include "G4VPrimitiveScorer.hh"
#include "SEPSEnergyDeposit.hh"
#include "SEPSLocation.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

SEDetectorConstruction::SEDetectorConstruction()
{
  DefineMaterials();
}

SEDetectorConstruction::~SEDetectorConstruction()
{ ; }

auto SEDetectorConstruction::Construct() -> G4VPhysicalVolume*
{
  return SetupBaseline();
}

void SEDetectorConstruction::DefineMaterials()
{
  G4NistManager* nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Galactic"); // vacuum

  // Diamond material
  G4double a;  // mass of a mole;
  G4double z;
  G4double density; 
  new G4Material("diamond", z=6., a=12.011*g/mole, density=3.515*g/cm3);

}

void SEDetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // Only need to construct the (per-thread) SD once
  if(!fSD.Get())
  {
    auto* diamondet = new G4MultiFunctionalDetector("Diamond");
    fSD.Put(diamondet);

    // Also only add it once to the SD manager!
    G4SDManager::GetSDMpointer()->AddNewDetector(diamondet);

    auto* eprimitive = new SEPSEnergyDeposit("Edep");
    diamondet->RegisterPrimitive(eprimitive);

    auto* primitive = new SEPSLocation("Loc");
    diamondet->RegisterPrimitive(primitive);

    SetSensitiveDetector("Det_log", diamondet);
  }

}

auto SEDetectorConstruction::SetupBaseline() -> G4VPhysicalVolume*
{
  // Get materials
  auto* worldMaterial = G4Material::GetMaterial("G4_Galactic");
  auto* detMat        = G4Material::GetMaterial("diamond");
    
  // size parameter
  // world
  G4double worldhside = 1.0 * CLHEP::cm;   // box half side in x, y
  G4double worldhZ    = 0.5 * CLHEP::cm;   // box half side in z
  
  // box with Diamond
  G4double halfx   = 2.55 * CLHEP::mm;    // 5.1 x 3.6 x 0.6 mm^3
  G4double halfy   = 1.8 * CLHEP::mm;
  G4double halfz   = 0.3 * CLHEP::mm;    
    
  // Volumes for this geometry
  
  //
  // World
  //
  auto* worldSolid = new G4Box("World", worldhside, worldhside, worldhZ);
  auto* worldLogical  = new G4LogicalVolume(worldSolid, worldMaterial, "World_log");
  auto* worldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), worldLogical,
                                           "World_phys", nullptr, false, 0);
                                
  //
  // Diamond box
  //
  auto* detSolid = new G4Box("DetSolid", halfx, halfy, halfz);
   
  
  // logical volumes    
  auto* detLogical = new G4LogicalVolume(detSolid, detMat, "Det_log");
 
  // placements, central
  new G4PVPlacement(nullptr, G4ThreeVector(0., 0., 0.),
                    detLogical, "Det_phys", worldLogical, false, 0, true);
  
  return worldPhysical;
}
