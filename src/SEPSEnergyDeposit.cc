// SEPSEnergyDeposit
#include "SEPSEnergyDeposit.hh"
#include "G4UnitsTable.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring total energy deposit
//
///////////////////////////////////////////////////////////////////////////////

SEPSEnergyDeposit::SEPSEnergyDeposit(G4String name, G4int depth)
: G4VPrimitiveScorer(std::move(name), depth)
, HCID(-1)
, EvtMap(nullptr)
{
  SetUnit("keV");
}

SEPSEnergyDeposit::SEPSEnergyDeposit(G4String name, const G4String& unit, G4int depth)
: G4VPrimitiveScorer(std::move(name), depth)
, HCID(-1)
, EvtMap(nullptr)
{
  SetUnit(unit);
}

SEPSEnergyDeposit::~SEPSEnergyDeposit() = default;

G4bool SEPSEnergyDeposit::ProcessHits(G4Step* aStep, G4TouchableHistory* /*unused*/)
{
  G4double edep = aStep->GetTotalEnergyDeposit();
  if (edep <= 0.0) return false;
  
  // G4int index = GetIndex(aStep);
  G4int index = aStep->GetTrack()->GetCurrentStepNumber(); // unique key
  // debug
  // G4cout << "key = " << index << " edep = " << edep / GetUnitValue() << G4endl;

  EvtMap->add(index, edep); // adds up total energy for each step

  return true;
}

void SEPSEnergyDeposit::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap = new G4THitsMap<G4double>(GetMultiFunctionalDetector()->GetName(), GetName());
  if(HCID < 0)
  {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*) EvtMap);
}

void SEPSEnergyDeposit::EndOfEvent(G4HCofThisEvent* /*unused*/) { ; }

void SEPSEnergyDeposit::clear() { EvtMap->clear(); }

void SEPSEnergyDeposit::DrawAll() { ; }

void SEPSEnergyDeposit::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  auto itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++)
  {
    G4cout << "  key: " << itr->first
           << "  deposited energy: " << *(itr->second) / GetUnitValue() << " [" << GetUnit()
           << "]" << G4endl;
  }
}

void SEPSEnergyDeposit::SetUnit(const G4String& unit)
{
  CheckAndSetUnit(unit, "Energy");
}
