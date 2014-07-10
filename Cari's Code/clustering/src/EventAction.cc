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
 * i.e. histograms or ntuples. Also include whatever verbose arguments
 * you want in your final read out.}
 *
 * 
 */

#include "EventAction.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <iomanip>

#include "Analysis.hh"

#include "G4PhysicalConstants.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"

#include "TestHit.hh"


EventAction::EventAction()
  : G4UserEventAction(),
    fDistance(0.) //distance from center of target to front of calorimeter
{
  // DetectorConstruction* detector = new DetectorConstruction();
  fDistance = 10*m;
}


EventAction::~EventAction()
{}

//!!!
//Reset your variables
void EventAction::BeginOfEventAction(const G4Event* /* run*/)
{
  G4cout << "Being of Event Action" << G4endl;

}

/*
 *Calculates the angle from the Z axis the photon hits the target
 *returns G4double angle in degrees
 */
G4double EventAction::CalcTheta(G4double x, G4double y)
{
  G4double distance;
  distance  = std::sqrt(x*x+y*y);

  G4float theta =  std::atan(distance/fDistance);


  return G4double(theta*180/pi);
}

//!!!
//Fill your data analysis
//Add verbosity
void EventAction::EndOfEventAction(const G4Event* event)
{
  G4cout << "End of Event Action" << G4endl;
  
  G4HCofThisEvent* hce = event->GetHCofThisEvent(); //hit collection array allows for several kinds of hits

  //analysis manager
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();

  //get ID for the calorimeter's hit collection
  G4int collectionID = fSDM->GetCollectionID("TestHitsCollection");

  TestHitsCollection* hitColl = static_cast<TestHitsCollection*>(hce->GetHC(collectionID));

  G4double eDep(0.);
  for (G4int i=9; i<49; i++)
    {
      for (G4int j=0; j<25; j++)
	{
	  TestHit* hit = (*hitColl)[(i-9)*25+j];
	  eDep = hit->GetEnergyDep();
	  if (eDep >0.5*MeV)
	    {
	      analysisMan->FillNtupleIColumn(i-9, j, eDep);
	      analysisMan->FillH2(0, j, i-9, 1);
	    }
      
	}
      analysisMan->AddNtupleRow(i-9);
    }
 

 
    
}

