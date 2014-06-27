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
// $Id: B4aSteppingAction.cc 68058 2013-03-13 14:47:43Z gcosmo $
// 
/// \file B4aSteppingAction.cc
/// \brief Implementation of the B4aSteppingAction class

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4StepPoint.hh"
#include "G4SystemOfUnits.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(
                      const DetectorConstruction* detectorConstruction,
                      EventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step

  // get volume of the current step
  G4VPhysicalVolume* prevolume 
    = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  G4VPhysicalVolume* postvolume 
    = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
  /*
  G4cout << "\n == Printing Step information == \n";
  if (step->GetPreStepPoint()->GetProcessDefinedStep()!=NULL)
    G4cout << step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
  if (step->GetPostStepPoint()->GetProcessDefinedStep()!=NULL)
    G4cout << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;


  */    
  G4int code = step->GetTrack()->GetDynamicParticle()->GetPDGcode();
  /*G4String processName;
  if (step->GetPreStepPoint()->GetProcessDefinedStep()!=NULL)
    processName = step->GetPreStepPoint()->GetProcessDefinedStep()->GetProcessName();
  //G4cout << code << G4endl;*/
  const double PI=3.14159265359;
  //G4cout << "PDGcode = " << code << G4endl;
  if (( prevolume == fDetConstruction->GetTargetPV() ) && ( postvolume != fDetConstruction->GetTargetPV()) && (code == 22)) {
  //if ((step->GetPreStepPoint()->GetProcessDefinedStep()==NULL) && (code == 22)) {
    //if (step->GetTrack()->GetParentID()==1) fEventAction->MarkMultiBrem();
    G4StepPoint* p=step->GetPostStepPoint();
    G4double px=p->GetMomentum().x();
    G4double py=p->GetMomentum().y();
    G4double pz=p->GetMomentum().z();
    G4double E=p->GetTotalEnergy();
    if (E > 1*MeV) {  //energy cut
      //direction cut
      G4double pt = pow(pow(px,2) + pow(py,2) , 0.5);
      G4double angle = atan (pt/pz) * 180/PI;
      //if ((angle > 3.0) && (angle < 4.0)) 
        fEventAction->StorePhoton(px,py,pz,E);

    } 
    //step->GetTrack()->SetTrackStatus(fStopAndKill);
    //G4cout << "Photon caught" << G4endl;
  }

  
  if (( prevolume == fDetConstruction->GetTargetPV() ) && ( postvolume != fDetConstruction->GetTargetPV()) && (code == -11)) {
  //if ((processName=="eBrem") && (code == -11)) {
    G4StepPoint* p=step->GetPreStepPoint();
    G4double px=p->GetMomentum().x();
    G4double py=p->GetMomentum().y();
    G4double pz=p->GetMomentum().z();
    G4double E=p->GetTotalEnergy();
    fEventAction->StorePositron(px,py,pz,E);
    //G4cout << "Positron caught" << G4endl;
    //step->GetTrack()->SetTrackStatus(fStopAndKill);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
