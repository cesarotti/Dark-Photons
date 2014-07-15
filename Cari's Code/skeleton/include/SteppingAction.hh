/*
 * Dark Photon Stepping Action
 * !!!History
 *    CJC 6.16.14 created
 *
 * 
 */

#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"

class DetectorConstruction;
class EventAction;

/*
 * Collect energy deposits and track lengths of charged particles
 * and update in EventAction
 */

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(const DetectorConstruction* detector, 
		 EventAction* eventAction);
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step* step);

private:
  const DetectorConstruction* fDetConstruction;
  EventAction* fEventAction;
};

#endif
