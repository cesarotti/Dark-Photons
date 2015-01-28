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
#include "H02PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

ActionInitialization::ActionInitialization()
  : G4VUserActionInitialization()
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

<<<<<<< HEAD
  SetUserAction(new PrimaryGeneratorAction);
=======
  SetUserAction(new H02PrimaryGeneratorAction);
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
  SetUserAction(new RunAction);
  SetUserAction(new EventAction);

  G4cout << "Action initializer complete" << G4endl;
}
