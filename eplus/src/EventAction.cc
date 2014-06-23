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

#include "CalorHit.hh"


EventAction::EventAction()
  : G4UserEventAction(),
    fDistance(0.) //distance from center of target to front of calorimeter
{
  DetectorConstruction* detector = new DetectorConstruction();
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
  G4int collectionID = fSDM->GetCollectionID("CalorimeterHitsCollection");

  CalorimeterHitsCollection* hitColl = static_cast<CalorimeterHitsCollection*>(hce->GetHC(collectionID));

  G4int numHits = hitColl->entries();
  
  analysisMan->FillNtupleIColumn(0,0, numHits);
  
  for (G4int i=0; (i<numHits) && (i<2); i++)
    {
      CalorHit* newHit = (*hitColl)[i];
      G4ThreeVector position = newHit->GetPos();
      G4cout << "x: " << position.x() << " y: " << position.y();
      analysisMan->FillNtupleDColumn(0,1+(i*3), position.x());
      analysisMan->FillNtupleDColumn(0, 2+(i*3), position.y());
      G4double totEnergy = newHit->GetTotalEnergy();
      analysisMan->FillNtupleDColumnx(0, 3+(i*3), totEnergy);
      analysisMan->FillNtupleDColumn(0, 7+(i%2), CalcTheta(position.x(), position.y()));
      analysisMan->FillNtupleIColumn(0, 9+i, newHit->GetCrystalNumber());
}

  analysisMan->AddNtupleRow();

  analysisMan->FillH1(0, numHits, 1.0);
  




}

