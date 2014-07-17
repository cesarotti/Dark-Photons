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

#include "BasicHit.hh"


EventAction::EventAction()
  : G4UserEventAction(),
    fDistance(0.) //distance from center of target to front of calorimeter
{
  // DetectorConstruction* detector = new DetectorConstruction();
  fDistance = 9.99*m;
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
G4double EventAction::CalcTheta(G4double x, G4double y, G4double z)
{
  G4double distance;
  distance  = std::sqrt(x*x+y*y);

  G4float theta =  std::atan(distance/(5*m+z));


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
  G4int collectionID = fSDM->GetCollectionID("BasicHitsCollection");

  BasicHitsCollection* hitColl = static_cast<BasicHitsCollection*>(hce->GetHC(collectionID));

  G4int numHits = hitColl->entries();
  G4int id;
  G4ThreeVector position;

  G4int numProton = 0;
  G4int numPiPlus = 0;
  G4int numPiMinius = 0;

  for (int i=0; i < numHits; i++) {
    BasicHit* hit = (*hitColl)[i];
    id = hit->GetPDGID();
    position = hit->GetPosition();


    if (id == 2212) {
      numProton++;
    } else if (id == 211) {
      numPiPlus++;
    } else if (id == -211) {
      numPiMinius++;
    } else if (id == 11) {
      analysisMan->FillNtupleDColumn(0, hit->GetTotalEnergy());
      analysisMan->FillNtupleDColumn(1, CalcTheta(position.getX(), position.getY(), position.getZ()));
    }
  }

  analysisMan->FillNtupleIColumn(2, numProton);
  analysisMan->FillNtupleIColumn(3, numPiPlus);
  analysisMan->FillNtupleIColumn(4, numPiMinius);

  analysisMan->AddNtupleRow();

  




}

