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
#include "OmniHit.hh"


EventAction::EventAction()
  : G4UserEventAction(), 
    fTHSD(-1), 
    fOHSD(-1)
{
}


EventAction::~EventAction()
{}

//!!!
//Reset your variables
void EventAction::BeginOfEventAction(const G4Event* /* run*/)
{
  if (fTHSD==-1)
    {
      G4SDManager* sdMan = G4SDManager::GetSDMpointer();
      fTHSD = sdMan->GetCollectionID("calorimeterSD/TestHitsCollection");
      fOHSD = sdMan->GetCollectionID("omniSD/OmniHitsCollection");
    }
}


//!!!
//Fill your data analysis
//Add verbosity
void EventAction::EndOfEventAction(const G4Event* event)
{
  
  G4HCofThisEvent* hce = event->GetHCofThisEvent(); //hit collection array allows for several kinds of hits 

  //analysis manager
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();

  //get ID for the calorimeter's hit collection
  //G4int collectionID = fSDM->GetCollectionID("TestHitsCollection");

  TestHitsCollection* testHitColl = static_cast<TestHitsCollection*>(hce->GetHC(fTHSD));

  OmniHitsCollection* omniHitColl = static_cast<OmniHitsCollection*>(hce->GetHC(fOHSD));
  
  G4double eDep(0.);
  G4bool hits = false;
  G4double total(0.);
 
  for (int i=0; i<1225; i++)
    {
	  TestHit* hit = (*testHitColl)[i];
	  eDep = hit->GetEnergyDep();
	  if (eDep > 0)
	    {
	      total+=eDep;
	      analysisMan->FillNtupleDColumn(i, eDep);
	      hits = true;
	    }
     
    }
  G4double energy(0.);
  
  OmniHit* hit2 = (*omniHitColl)[0];
  energy = hit2->GetTotalEnergy(); 
  if (energy > 0.)
    {
      analysisMan->FillNtupleDColumn(1226, energy); 
      analysisMan->FillNtupleDColumn(1227, hit2->GetPos().getX());
      analysisMan->FillNtupleDColumn(1228, hit2->GetPos().getY());
      analysisMan->FillNtupleDColumn(1229, 
    }
  

if (hits) {analysisMan->FillNtupleIColumn(1225, 1);}
analysisMan->AddNtupleRow(); 
  
}

