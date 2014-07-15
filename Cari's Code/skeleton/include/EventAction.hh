/*
 *Dark Photon Event Action
 *
 *!!!History
 *   CJC 6.15.14 created
 *
 */

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "globals.hh"


class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  virtual void BeginOfEventAction(const G4Event* );
  virtual void EndOfEventAction(const G4Event* );

  void AddEn(G4double de, G4double dl);

private:
  //Variables to keep track of in data storage
G4double fEdep;
  G4double fTrackAbs;

};
 #endif
