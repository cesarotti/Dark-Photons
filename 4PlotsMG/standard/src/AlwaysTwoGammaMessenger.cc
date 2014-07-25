

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
/// \file eventgenerator/HepMC/HepMCEx02/src/AlwaysTwoGammaMessenger.cc
/// \brief Implementation of the AlwaysTwoGammaMessenger class
//
//   $Id: AlwaysTwoGammaMessenger.cc 77801 2013-11-28 13:33:20Z gcosmo $
//
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "AlwaysTwoGammaMessenger.hh"
#include "H02PrimaryGeneratorAction.hh"
#include "AlwaysTwoGamma.hh"
#include "G4ProcessManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AlwaysTwoGammaMessenger::AlwaysTwoGammaMessenger(PositronPhysicsList1* physlist)
{

  protonslist = physlist;

  //Load this
  dir= new G4UIdirectory("/madgraph/");
  dir-> SetGuidance("Control commands for selecting madgraph imports");

  //verbose= new G4UIcmdWithAnInteger("/generator/verbose", this);
  //verbose-> SetGuidance("set verbose level (0,1,2)");
  //verbose-> SetParameterName("verbose", false, false);
  //verbose-> SetDefaultValue(0);
  //verbose-> SetRange("verbose>=0 && verbose<=2");

  enable= new G4UIcmdWithAString("/madgraph/enable", this);
  enable-> SetGuidance("enable madgraph file");
  enable-> SetParameterName("madgraph_enable_file", true,true);
  enable-> SetDefaultValue("particleGun");

  disable= new G4UIcmdWithAString("/madgraph/disable", this);
  disable-> SetGuidance("enable madgraph file");
  disable-> SetParameterName("madgraph_disable_file", false, false);
  disable-> SetCandidates("twogamma threegamma");
  disable-> SetDefaultValue("particleGun");
}

inline bool exists (const G4String* name) {
    chdir("madgraph_processes");
    if (FILE *file = fopen(name->data(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
AlwaysTwoGammaMessenger::~AlwaysTwoGammaMessenger()
{
  //delete verbose;
  delete select;

  delete dir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void AlwaysTwoGammaMessenger::SetNewValue(G4UIcommand* command,
                                              G4String newValues)
{
  if ( command==enable) {
    if(exists(&newValues)){
      if(enabled_files.find(newValues) == enabled_files.end()){
        G4VEmProcess* new_process = new AlwaysTwoGamma(newValues);
        new_process->SetCrossSectionBiasingFactor(1e+06, true); 
        enabled_files.insert ( std::pair<G4String, G4VEmProcess*>(newValues,new_process) );
        protonslist->pman->AddProcess(new_process, 0, -1, 4);
      } else{
        G4cout << "Already loaded" << G4endl;
      }
      
    } else {
      G4cout << "Could not find taht file" << G4endl;
    }
    G4cout << "Currently enabled files:" << G4endl;

    //for (std::pair<G4String, G4VEmProcess*>& x: enabled_files) {
    //       G4cout <<  x.first << G4endl;
    //}  
    
  } else {
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String AlwaysTwoGammaMessenger::GetCurrentValue(G4UIcommand* command)
{
  G4String cv, st;
  if (command == select) {
  }

 return cv;
}


