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
/// \file eventgenerator/HepMC/HepMCEx02/src/H02PrimaryGeneratorAction.cc
/// \brief Implementation of the H02PrimaryGeneratorAction class
//   $Id: H02PrimaryGeneratorAction.cc 77801 2013-11-28 13:33:20Z gcosmo $
//
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "HepMCG4AsciiReader.hh"
#include "H02PrimaryGeneratorAction.hh"
#include "H02PrimaryGeneratorMessenger.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSAngDistribution.hh"
#include "G4SingleParticleSource.hh"
#include "G4GeneralParticleSource.hh"
//#include "AlwaysTwoGammaMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
H02PrimaryGeneratorAction::H02PrimaryGeneratorAction()
{
  // default generator is particle gun.
  currentGenerator= particleGun= fParticleSource= new G4GeneralParticleSource();

  // default particle kinematic

  G4ParticleDefinition* particleDefinition
    //!!!
    //Positron beam
    = G4ParticleTable::GetParticleTable()->FindParticle("e+");

  fParticleSource->GetCurrentSource()->SetParticleDefinition(particleDefinition);

  fParticleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0,0,-8*m));
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosRot1(G4ThreeVector(1,0,0));
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosRot2(G4ThreeVector(0,1,0));

  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisType("Beam");
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisShape("Circle");

  fParticleSource->GetCurrentSource()->GetPosDist()->SetRadius(1e-6*mm);
  fParticleSource->GetCurrentSource()->GetPosDist()->SetBeamSigmaInR(1*mm);
   
  fParticleSource->GetCurrentSource()->GetAngDist()->SetAngDistType("beam1d");
  fParticleSource->GetCurrentSource()->GetAngDist()->DefineAngRefAxes("angref1", G4ThreeVector(-1,0,0));
  fParticleSource->GetCurrentSource()->GetAngDist()->DefineAngRefAxes("angref2", G4ThreeVector(0,1,0));

  fParticleSource->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Lin");
  G4double max=5*GeV;
  G4double range=50*MeV;
  G4double mass=particleDefinition->GetPDGMass();
  fParticleSource->GetCurrentSource()->GetEneDist()->SetEmax(max-mass);
  fParticleSource->GetCurrentSource()->GetEneDist()->SetEmin(max-range-mass);
  fParticleSource->GetCurrentSource()->GetEneDist()->SetGradient(0);
  fParticleSource->GetCurrentSource()->GetEneDist()->SetInterCept(1);

  currentGeneratorName= "particleGun";
  hepmcAscii= new HepMCG4AsciiReader();

  gentypeMap["particleGun"]= particleGun;
  gentypeMap["hepmcAscii"]= hepmcAscii;

  messenger= new H02PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
H02PrimaryGeneratorAction::~H02PrimaryGeneratorAction()
{
  delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void H02PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(currentGenerator)
    currentGenerator-> GeneratePrimaryVertex(anEvent);
  else
    G4Exception("H02PrimaryGeneratorAction::GeneratePrimaries",
                "InvalidSetup", FatalException,
                "Generator is not instanciated.");
}
