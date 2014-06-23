//
/*
 *Dark Photon Action Initialization 
 *!!!History 
 *   CJC 6.15.14 created
 * 
 *
 *file: ActionInitialization.cc
 *
 * Description:
 * Initiates necessary classes
 */

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction *detConstruction)
 : G4VUserActionInitialization(), fDetConstruction(detConstruction)
{}

ActionInitialization::~ActionInitialization()
{}

// Creates instances of the necessary classes for running
void ActionInitialization::BuildForMaster() const
{
 
  SetUserAction(new RunAction);
}


//Creates instances of the necessary classes for Geant
void ActionInitialization::Build() const
{

  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  EventAction* eventAction = new EventAction();
  SetUserAction(eventAction);
  SetUserAction(new SteppingAction(fDetConstruction,eventAction));

  G4cout << "Action initializer complete" << G4endl;
}
