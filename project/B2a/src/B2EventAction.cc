//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: B2EventAction.cc 75214 2013-10-29 16:04:42Z gcosmo $
//
/// \file B2EventAction.cc
/// \brief Implementation of the B2EventAction class

#include "B2EventAction.hh"
#include "B2CalorHit.hh" //CJC 6.9.14
#include "B2TrackerHit.hh"

#include "Analysis.hh" //CJC 6.13.14
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <iomanip>

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2EventAction::B2EventAction()
  : G4UserEventAction(),
    fEdep(0.),
    fEnergyGap(0.), 
    fTrackLAbs(0. ),
    fTrackLGap(0. )
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2EventAction::~B2EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2EventAction::BeginOfEventAction(const G4Event* run)
{
  fEdep = 0.0;
  fEnergyGap =0.0;
  fTrackLAbs = 0.0;
  fTrackLGap = 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2EventAction::EndOfEventAction(const G4Event* event)
{
  //Histogrammin'
  //CJC 6.13.14
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance(); 
  analysisMan->FillH1(0, fEdep);



  //YE WHO ENTER BEWARE VERBOSITY 


  // get number of stored trajectories
  G4cout << "End of Event Action" << G4endl;

  G4TrajectoryContainer* trajectoryContainer = event->GetTrajectoryContainer();
  G4int n_trajectories = 0;
  if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
  
  // periodic printing
  
  G4int eventID = event->GetEventID();
  if ( eventID < 100 || eventID % 100 == 0) {
    G4cout << ">>> Event: " << eventID  << G4endl;
    if ( trajectoryContainer ) {
      G4cout << "    " << n_trajectories
             << " trajectories stored in this event." << G4endl;
    }

    G4SDManager* fSDM = G4SDManager::GetSDMpointer();

    G4int collectionID = fSDM->GetCollectionID("TrackerHitsCollection");
    G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(collectionID);

    //G4cout << "    " << hc->GetSize() << " tracker hits stored in this event" <<
    //G4endl;
   
    /*
     G4VHitsCollection* hc = event->GetHCofThisEvent()->GetHC(0);
    G4cout << "    "  
    << hc->GetSize() << " hits stored in this event" << G4endl;
    */
  }
  
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();

  G4int collectionID = fSDM->GetCollectionID("CalorimeterHitsCollection");
  G4VHitsCollection* hc2 = event->GetHCofThisEvent()->GetHC(collectionID);

  G4cout << "    " << hc2->GetSize() << " calorimeter hits sorted in this event"
	 << G4endl;
  for (int i=0; i<(hc2->GetSize()); i++)
    {
      hc2->GetHit(i)->Print();
      G4cout << G4endl;
    }
  


}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
