//
/*
 *Dark Photon Action Initialization 
 *!!!History 
 *   CJC 6.15.14 created
 *
 *file: ActionInitialization.cc
 */

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

ActionInitialization::ActionInitialization()
  : G4VUserActionInitialization()
{}

ActionInitialization::~ActionInitialization()
{}

// Creates instances of multiple classes
void ActionInitialization::BuildForMaster() const
{
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  SetUserAction(new EventAction);
}

void ActionInitialization::Build() const
{
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  SetUserAction(new EventAction);
}
