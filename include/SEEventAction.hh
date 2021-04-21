#ifndef SEEventAction_h
#define SEEventAction_h 1

#include "G4THitsMap.hh"
#include "G4UserEventAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

/// Event action class
///

class SEEventAction : public G4UserEventAction
{
public:
  SEEventAction()          = default;
  virtual ~SEEventAction() = default;

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

private:
  // methods
  G4THitsMap<G4double>*      GetHitsCollection(G4int hcID, const G4Event* event) const;
  G4THitsMap<G4ThreeVector>* GetVecHitsCollection(G4int hcID, const G4Event* event) const;

  // data members
  // hit data
  G4int                 fEdepID   = -1;
  G4int                 fLocID    = -1;

};

#endif
