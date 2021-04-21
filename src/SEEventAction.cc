#include "SEEventAction.hh"
#include "g4root.hh"

#include <vector>

#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4UnitsTable.hh"
#include "G4ios.hh"


G4THitsMap<G4double>* SEEventAction::GetHitsCollection(G4int          hcID,
                                                         const G4Event* event) const
{
  auto hitsCollection =
    static_cast<G4THitsMap<G4double>*>(event->GetHCofThisEvent()->GetHC(hcID));

  if(hitsCollection == nullptr)
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("SEEventAction::GetHitsCollection()", "MyCode0003", FatalException,
                msg);
  }

  return hitsCollection;
}

G4THitsMap<G4ThreeVector>* SEEventAction::GetVecHitsCollection(
  G4int hcID, const G4Event* event) const
{
  auto hitsCollection =
    static_cast<G4THitsMap<G4ThreeVector>*>(event->GetHCofThisEvent()->GetHC(hcID));

  if(hitsCollection == nullptr)
  {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("SEEventAction::GetVecHitsCollection()", "MyCode0003", FatalException,
                msg);
  }

  return hitsCollection;
}


void SEEventAction::BeginOfEventAction(const G4Event*
                                         /*event*/)
{ ; }

void SEEventAction::EndOfEventAction(const G4Event* event)
{
  // Get GAS hits collections IDs
  if(fEdepID < 0)
  {
    fEdepID   = G4SDManager::GetSDMpointer()->GetCollectionID("Diamond/Edep");
    fLocID    = G4SDManager::GetSDMpointer()->GetCollectionID("Diamond/Loc");
  }

  // Get entries from hits collections
  //
  G4THitsMap<G4double>*      EdepMap   = GetHitsCollection(fEdepID, event);
  G4THitsMap<G4ThreeVector>* LocMap    = GetVecHitsCollection(fLocID, event);

  if(EdepMap->entries() <= 0)
  {
    return;  // no action on no hit
  }

  // dummy storage
  std::vector<double> tedep, tx, ty, tz;

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // fill Hits output from SD
  for(auto it : *EdepMap->GetMap())
  { 
    double ed = (*it.second) / G4Analysis::GetUnitValue("keV");
    tedep.push_back(ed);
  }
  for(auto it : *LocMap->GetMap())
  {
    double x = (*it.second).x() / G4Analysis::GetUnitValue("mm");
    double y = (*it.second).y() / G4Analysis::GetUnitValue("mm");
    double z = (*it.second).z() / G4Analysis::GetUnitValue("mm");

    tx.push_back(x);
    ty.push_back(y);
    tz.push_back(z);
  }

  // fill the ntuple
  G4int eventID = event->GetEventID();
  for (unsigned int i=0;i<tedep.size();i++)
  {
    analysisManager->FillNtupleIColumn(0, eventID); // repeat all rows
    analysisManager->FillNtupleDColumn(1, tedep.at(i));
    analysisManager->FillNtupleDColumn(2, tx.at(i));
    analysisManager->FillNtupleDColumn(3, ty.at(i));
    analysisManager->FillNtupleDColumn(4, tz.at(i)); // same size
    analysisManager->AddNtupleRow();
  }

}
