/*
 * Dark Photon Event Action
 *!!!History
 *   CJC 6.15.14 created
 *   CJC 6.16.14 analysis added
 *
 * file: EventAction.cc
 *
 * Description: 
 * Event Action method is called after every event. We are using it 
 * to store data from event to event in large runs.
 * Methods to update:
 *
 * BeginOfEventAction
 * {Reset the variables you want to keep track of}
 *
 * EndOfEventAction
 * {fill all of the data storage methods with the analysis manager, 
 * i.e. hisotgrams or ntuples. Also include whatever verbose arguments
 * you want in your final read out.}
 *
 *Variables:
 *fEdep
 *fTrackAbs
 * 
 */

#include "EventAction.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <iomanip>

#include "Analysis.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"

EventAction::EventAction()
  : G4UserEventAction(),
    fEdep(0.), 
    fTrackAbs(0. )
{
}


EventAction::~EventAction()
{}

//!!!
//Reset your variables
void EventAction::BeginOfEventAction(const G4Event* run)
{
  G4cout << "Being of Event Action" << G4endl;
  fEdep = 0.0;
  fTrackAbs = 0.0;

}

//!!!
//Fill your data analysis
//Add verbosity
void EventAction::EndOfEventAction(const G4Event* event)
{
  G4cout << "End of Event Action" << G4endl;
  /*
  G4HCofThisEvent* hce = event->GetHCofThisEvent(); //hit collection array allows for several kinds of hits

  //analysis manager
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  */
  //get the hit collection desired and look through results

  //always finish ntuples with AddNtupleRow
  




}
