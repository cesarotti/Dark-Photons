/*
 *
 * Dark Photon Stepping Action
 * !!!History:
 *    CJC 6.16.14 created
 *
 * file: SteppingAction.cc
 *
 *Description :
 * holds information regarding the track length of charged particles and
 * the energy deposited once a sensitive detector is defined.
 */

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction (
				const DetectorConstruction* detector, 
				EventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detector),
    fEventAction(eventAction)
{
}


SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Collect energy and track length step by step

  G4cout << "Stepping Action" << G4endl;

  // get volume of the current step
G4VPhysicalVolume* volume
  = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();

// energy deposit
 G4double edep = step->GetTotalEnergyDeposit();

 // step length
 G4double stepLength = 0.;
 if ( step->GetTrack()->GetDefinition()->GetPDGCharge() !=0.)//charged particles
   { 
     stepLength = step->GetStepLength();
   }

}
