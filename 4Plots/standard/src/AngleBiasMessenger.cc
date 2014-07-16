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
/// \file electromagnetic/TestEm5/src/AngleBiasMessenger.cc
/// \brief Implementation of the AngleBiasMessenger class
//
// $Id: AngleBiasMessenger.cc 77083 2013-11-21 10:35:55Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "AngleBiasMessenger.hh"

#include "NewBremsstrahlungRelModel.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AngleBiasMessenger::AngleBiasMessenger(NewBremsstrahlungRelModel* pBrem)
:G4UImessenger(),fBremRelModel(pBrem),
 fPhysDir(0),    
 fThetaHighCmd(0),    
 fThetaLowCmd(0),
 fEnergyHighCmd(0),
 fEnergyLowCmd(0)
{
  fPhysDir = new G4UIdirectory("/brem/");
  fPhysDir->SetGuidance("bremsstrahlung bias commands");

  fThetaHighCmd = new G4UIcmdWithADoubleAndUnit("/brem/setHighTheta",this);  
  fThetaHighCmd->SetGuidance("Set highest theta.");
  fThetaHighCmd->SetParameterName("THigh",false);
  fThetaHighCmd->SetUnitCategory("Angle");
  fThetaHighCmd->SetRange("THigh>0.0");
  fThetaHighCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fThetaHighCmd->SetToBeBroadcasted(false);

  fThetaLowCmd = new G4UIcmdWithADoubleAndUnit("/brem/setLowTheta",this);  
  fThetaLowCmd->SetGuidance("Set lowest theta.");
  fThetaLowCmd->SetParameterName("TLow",false);
  fThetaLowCmd->SetUnitCategory("Angle");
  fThetaLowCmd->SetRange("TLow>0.0");
  fThetaLowCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fThetaLowCmd->SetToBeBroadcasted(false);

  fEnergyHighCmd = new G4UIcmdWithADoubleAndUnit("/brem/setHighEnergy",this);  
  fEnergyHighCmd->SetGuidance("Set highest Energy.");
  fEnergyHighCmd->SetParameterName("EHigh",false);
  fEnergyHighCmd->SetUnitCategory("Energy");
  fEnergyHighCmd->SetRange("EHigh>0.0");
  fEnergyHighCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fEnergyHighCmd->SetToBeBroadcasted(false);

  fEnergyLowCmd = new G4UIcmdWithADoubleAndUnit("/brem/setLowEnergy",this);  
  fEnergyLowCmd->SetGuidance("Set lowest energy.");
  fEnergyLowCmd->SetParameterName("ELow",false);
  fEnergyLowCmd->SetUnitCategory("Energy");
  fEnergyLowCmd->SetRange("ELow>0.0");
  fEnergyLowCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fEnergyLowCmd->SetToBeBroadcasted(false);
  G4cout << "\n\n\n here!!! \n\n\n\n" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

AngleBiasMessenger::~AngleBiasMessenger()
{
  delete fThetaHighCmd;
  delete fThetaLowCmd;
  delete fEnergyHighCmd;
  delete fEnergyLowCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void AngleBiasMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if( command == fThetaHighCmd )
   { fBremRelModel->SetHighestTheta(fThetaHighCmd->GetNewDoubleValue(newValue));}
    
  if( command == fThetaLowCmd ) {
   fBremRelModel->SetLowestTheta(fThetaLowCmd->GetNewDoubleValue(newValue));
  }
     
  if( command == fEnergyHighCmd ) {
   fBremRelModel->SetHighestEnergy(fEnergyHighCmd->GetNewDoubleValue(newValue));
  }

  if( command == fEnergyLowCmd ) {
    fBremRelModel->SetLowestEnergy(fEnergyLowCmd->GetNewDoubleValue(newValue));
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
