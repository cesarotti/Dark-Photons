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
// $Id: B2RunAction.cc 75214 2013-10-29 16:04:42Z gcosmo $
//
/// \file B2RunAction.cc
/// \brief Implementation of the B2RunAction class

#include "B2RunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "Analysis.hh" //CJC 6.13.14
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2RunAction::B2RunAction()
 : G4UserRunAction()
{ 
  // set printing event number per each 100 events
  G4RunManager::GetRunManager()->SetPrintProgress(1000);   

  //Analysis
  //CJC 6.13.14
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
  analysisMan->SetFirstHistoId(0);

  analysisMan->CreateH1("0", "Edep in calorimeter", 100, 1.0*MeV, 100.0*MeV);
  
  analysisMan->CreateNtuple("Positron Annihilation", "Photon Hits");
  analysisMan->CreateNtupleIColumn("Number of Hits"); 
  analysisMan->CreateNtupleDColumn("X Position"); 
  analysisMan->CreateNtupleDColumn("Y Position");
  analysisMan->FinishNtuple();
  


  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B2RunAction::~B2RunAction()
{
  delete G4AnalysisManager::Instance();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2RunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
 //Data Storage
  G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();

  //Open an output file
  G4String fileName = "Positron Annihilation to two Gammas";
  analysisMan->OpenFile(fileName);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B2RunAction::EndOfRunAction(const G4Run* )
{
G4cout << "End of Run Action " << G4endl;

//Histogrammin'
//CJC 6.13.14
G4AnalysisManager* analysisMan = G4AnalysisManager::Instance();
analysisMan->Write();
analysisMan->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......