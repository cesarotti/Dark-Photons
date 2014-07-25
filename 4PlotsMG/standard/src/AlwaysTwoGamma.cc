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
// $Id: AlwaysTwoGamma.cc 68797 2013-04-05 13:27:11Z gcosmo $
//
// -------------------------------------------------------------------
//
// GEANT4 Class file
//
//
// File name:     AlwaysTwoGamma
//
// Author:        Vladimir Ivanchenko on base of Michel Maire code
//
// Creation date: 02.08.2004
//
// Modifications:
// 08-11-04 Migration to new interface of Store/Retrieve tables (V.Ivanchenko)
// 08-04-05 Major optimisation of internal interfaces (V.Ivanchenko)
// 03-05-05 suppress Integral option (mma)
// 04-05-05 Make class to be default (V.Ivanchenko)
// 25-01-06 remove cut dependance in AtRestDoIt (mma)
// 09-08-06 add SetModel(G4VEmModel*) (mma)
// 12-09-06, move SetModel(G4VEmModel*) in G4VEmProcess (mma)
// 30-05-12 propagate parent weight to secondaries (D. Sawkey)
//

//
// -------------------------------------------------------------------
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "AlwaysTwoGamma.hh"
#include "G4PhysicalConstants.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"
#include "G4Positron.hh"
#include "AlwaysTwoGammaModel.hh"
#include <sstream>
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

AlwaysTwoGamma::AlwaysTwoGamma(const G4String& name)
  : G4VEmProcess(name), isInitialised(false)
{

  G4cout << "Loaded AlwaysTwoGamma " << G4endl;

  name_of_model = name;
  std::ifstream input((name + "_config").data());
  G4double cross;
  G4int particle_pdg;
  
  //while (input >> particle_pdg)
  // {
  //     SetSecondaryParticle
  // }
  input >> cross;

  G4cout << "READ CROSS SECTION FROM FILE:" << cross << G4endl;
  G4cout << "NAME OF MODEL IS " << name_of_model << G4endl;

  theGamma = G4Gamma::Gamma();
  SetIntegral(true);
  SetBuildTableFlag(false);
  SetStartFromNullFlag(false);
  SetSecondaryParticle(theGamma);
  SetProcessSubType(fAnnihilation);
  enableAtRestDoIt = true;

  G4cout << "Finished constructor of AlwaysTwoGamma " << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

AlwaysTwoGamma::~AlwaysTwoGamma()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool AlwaysTwoGamma::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double AlwaysTwoGamma::AtRestGetPhysicalInteractionLength(
                              const G4Track&, G4ForceCondition* condition)
{
  *condition = NotForced;
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AlwaysTwoGamma::InitialiseProcess(const G4ParticleDefinition*)
{

  G4cout << "About to initialize model " << G4endl;
  if(!isInitialised) {
    isInitialised = true;
    G4cout << "ADDRESS NAME OF MODEL IN INITIALIZE " << name_of_model << G4endl;
    if(!EmModel(1)) { SetEmModel(new AlwaysTwoGammaModel(0, name_of_model),1); }
    EmModel(1)->SetLowEnergyLimit(MinKinEnergy());
    EmModel(1)->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, EmModel(1));
  }
  G4cout << "Done initialize model " << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void AlwaysTwoGamma::PrintInfo()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* AlwaysTwoGamma::AtRestDoIt(const G4Track& aTrack,
						   const G4Step& )
//
// Performs the e+ e- annihilation when both particles are assumed at rest.
// It generates two back to back photons with energy = electron_mass.
// The angular distribution is isotropic.
// GEANT4 internal units
//
// Note : Effects due to binding of atomic electrons are negliged.
{
  fParticleChange.InitializeForPostStep(aTrack);

  G4double cosTeta = 2.*G4UniformRand()-1.; 
  G4double sinTeta = sqrt((1.-cosTeta)*(1.0 + cosTeta));
  G4double phi     = twopi * G4UniformRand();
  G4ThreeVector dir(sinTeta*cos(phi), sinTeta*sin(phi), cosTeta);
  phi = twopi * G4UniformRand();
  G4ThreeVector pol(cos(phi), sin(phi), 0.0);
  pol.rotateUz(dir);
  
  // e+ parameters
  G4double weight = aTrack.GetWeight();
  G4double time   = aTrack.GetGlobalTime();

  // add gammas
  fParticleChange.SetNumberOfSecondaries(2);
  G4DynamicParticle* dp = 
    new G4DynamicParticle(theGamma, dir, electron_mass_c2);
  dp->SetPolarization(pol.x(),pol.y(),pol.z());
  G4Track* track = new G4Track(dp, time, aTrack.GetPosition());
  track->SetTouchableHandle(aTrack.GetTouchableHandle());
  track->SetWeight(weight); 
  pParticleChange->AddSecondary(track);

  dp = new G4DynamicParticle(theGamma,-dir, electron_mass_c2);
  dp->SetPolarization(-pol.x(),-pol.y(),-pol.z()); 
  track = new G4Track(dp, time, aTrack.GetPosition());
  track->SetTouchableHandle(aTrack.GetTouchableHandle());
  track->SetWeight(weight); 
  pParticleChange->AddSecondary(track);

  // Kill the incident positron
  //
  fParticleChange.ProposeTrackStatus(fStopAndKill);
  G4cout << "#######ATRESTCALLED"<< G4endl;
  return &fParticleChange;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
