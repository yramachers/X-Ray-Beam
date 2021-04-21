// SEPSLocation
#include "SEPSLocation.hh"
#include "G4UnitsTable.hh"

////////////////////////////////////////////////////////////////////////////////
// Description:
//   This is a primitive scorer class for scoring energy deposit location
//
///////////////////////////////////////////////////////////////////////////////

SEPSLocation::SEPSLocation(G4String name, G4int depth)
: G4VPrimitiveScorer(std::move(name), depth)
, HCID(-1)
, EvtMap(nullptr)
{
  SetUnit("mm");
}

SEPSLocation::SEPSLocation(G4String name, const G4String& unit, G4int depth)
: G4VPrimitiveScorer(std::move(name), depth)
, HCID(-1)
, EvtMap(nullptr)
{
  SetUnit(unit);
}

SEPSLocation::~SEPSLocation() = default;

G4bool SEPSLocation::ProcessHits(G4Step* aStep, G4TouchableHistory* /*unused*/)
{
  if(aStep->GetTotalEnergyDeposit() <= 0.0)
    return false;

  // G4int index = GetIndex(aStep);
  G4int index = aStep->GetTrack()->GetCurrentStepNumber(); // unique key
  G4ThreeVector loc;  // location at track creation

  G4bool killflag = false;
  if ( aStep->GetTrack()->GetTrackStatus() != fStopAndKill )
    loc = aStep->GetPreStepPoint()->GetPosition();
  else {
    loc = aStep->GetPostStepPoint()->GetPosition();
    killflag = true;
  }

  // debug
  // G4cout << "key: " << index << " location x=" << loc.x() / GetUnitValue()
  //        << ", y=" << loc.y() / GetUnitValue()<< G4endl;

  // insert in map only if new
  if (EvtMap->GetContainer()->find(index) == EvtMap->GetContainer()->end())  
    EvtMap->add(index, loc);
  else {
    if (killflag)
      EvtMap->set(index, loc); // overwrite final position if stopped
  }

  return true;
}

void SEPSLocation::Initialize(G4HCofThisEvent* HCE)
{
  EvtMap =
    new G4THitsMap<G4ThreeVector>(GetMultiFunctionalDetector()->GetName(), GetName());
  if(HCID < 0)
  {
    HCID = GetCollectionID(0);
  }
  HCE->AddHitsCollection(HCID, (G4VHitsCollection*) EvtMap);
}

void SEPSLocation::EndOfEvent(G4HCofThisEvent* /*unused*/) { ; }

void SEPSLocation::clear()
{
  EvtMap->clear();
}

void SEPSLocation::DrawAll() { ; }

void SEPSLocation::PrintAll()
{
  G4cout << " MultiFunctionalDet  " << detector->GetName() << G4endl;
  G4cout << " PrimitiveScorer " << GetName() << G4endl;
  G4cout << " Number of entries " << EvtMap->entries() << G4endl;
  auto itr = EvtMap->GetMap()->begin();
  for(; itr != EvtMap->GetMap()->end(); itr++)
  {
    G4cout << "  key: " << itr->first << "  energy deposit at: ("
           << (*(itr->second)).x() / GetUnitValue() << ", "
           << (*(itr->second)).y() / GetUnitValue() << ", "
           << (*(itr->second)).z() / GetUnitValue() << ")"
           << " [" << GetUnit() << "]" << G4endl;
  }
}

void SEPSLocation::SetUnit(const G4String& unit) { CheckAndSetUnit(unit, "Length"); }
