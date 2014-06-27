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

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"

#include "CalorHit.hh"

EventAction::EventAction()
  : G4UserEventAction()
{}


EventAction::~EventAction()
{}

//!!!
//Reset your variables
void EventAction::BeginOfEventAction(const G4Event* /* run*/)
{
  //G4cout << "Being of Event Action" << G4endl;
  fOneHit=0;
  StorePhoton(0,0,0,0);
  StorePositron(0,0,0,0);
}

//!!!
//Fill your data analysis
//Add verbosity
void EventAction::EndOfEventAction(const G4Event* event)
{
  //G4cout << "End of Event Action" << G4endl;
  
  //G4HCofThisEvent* hce = event->GetHCofThisEvent(); //hit collection array allows for several kinds of hits

  //analysis manager
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

   if ((fPhotonE!=0) && (fPositronE!=0))  {
    
    // fill ntuple
    analysisMan->FillNtupleDColumn(0, fPhotonE/MeV);
    analysisMan->FillNtupleDColumn(1, fPhotonPx/MeV);
    analysisMan->FillNtupleDColumn(2, fPhotonPy/MeV);
    analysisMan->FillNtupleDColumn(3, fPhotonPz/MeV);

    analysisMan->FillNtupleDColumn(4, fPositronE/MeV);
    analysisMan->FillNtupleDColumn(5, fPositronPx/MeV);
    analysisMan->FillNtupleDColumn(6, fPositronPy/MeV);
    analysisMan->FillNtupleDColumn(7, fPositronPz/MeV);

    analysisMan->AddNtupleRow();  
  }
  /*
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
      analysisMan->FillNtupleDColumn(0, 1+(i*6), position.x()/m);
      analysisMan->FillNtupleDColumn(0, 2+(i*6), position.y()/m);
      G4double totEnergy = newHit->GetTotalEnergy();
      analysisMan->FillNtupleDColumn(0, 3+(i*6), totEnergy/MeV);
      G4ThreeVector momentum = newHit->GetMomentum();
      analysisMan->FillNtupleDColumn(0, 4+(i*6), momentum.x()/MeV);
      analysisMan->FillNtupleDColumn(0, 5+(i*6), momentum.y()/MeV);
      analysisMan->FillNtupleDColumn(0, 6+(i*6), momentum.z()/MeV);
}

  analysisMan->AddNtupleRow();

  analysisMan->FillH1(0, numHits, 1.0);
  */
  




}

