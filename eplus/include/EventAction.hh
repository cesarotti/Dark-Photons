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
  
  void StorePhoton(G4double px, G4double py, G4double pz, G4double E);
  void StorePositron(G4double px, G4double py, G4double pz, G4double E);
  void MarkMultiBrem();

  private:
  	G4int fOneHit;
  	G4double fPhotonPx;
  	G4double fPhotonPy;
  	G4double fPhotonPz;
  	G4double fPhotonE;
  	G4double fPositronPx;
  	G4double fPositronPy;
  	G4double fPositronPz;
  	G4double fPositronE;  	

};

inline void EventAction::StorePhoton(G4double px, G4double py, G4double pz, G4double E){
	fPhotonPx=px;
	fPhotonPy=py;
	fPhotonPz=pz;
	fPhotonE=E;
}

inline void EventAction::StorePositron(G4double px, G4double py, G4double pz, G4double E){
	fPositronPx=px;
	fPositronPy=py;
	fPositronPz=pz;
	fPositronE=E;
}

inline void EventAction::MarkMultiBrem(){
	fOneHit++;
}


 #endif
