#include "G4eplusAnnihilationHi.hh"
#include "G4PhysicalConstants.hh"
#include "G4MaterialCutsCouple.hh"
#include "G4Gamma.hh"
#include "G4PhysicsVector.hh"
#include "G4PhysicsLogVector.hh"
#include "G4eeToTwoGammaModel.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

using namespace std;

G4eplusAnnihilationHi::G4eplusAnnihilationHi(const G4String& name)
  : G4VEmProcess(name), isInitialised(false)
{
  theGamma = G4Gamma::Gamma();
  SetIntegral(true);
  SetBuildTableFlag(false);
  SetStartFromNullFlag(false);
  SetSecondaryParticle(theGamma);
  SetProcessSubType(fAnnihilation);
  enableAtRestDoIt = true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4eplusAnnihilationHi::~G4eplusAnnihilationHi()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4bool G4eplusAnnihilationHi::IsApplicable(const G4ParticleDefinition& p)
{
  return (&p == G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4double G4eplusAnnihilationHi::AtRestGetPhysicalInteractionLength(
                              const G4Track&, G4ForceCondition* condition)
{
  *condition = NotForced;
  return 0.0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eplusAnnihilationHi::InitialiseProcess(const G4ParticleDefinition*)
{
  if(!isInitialised) {
    isInitialised = true;
    if(!EmModel(1)) { SetEmModel(new G4eeToTwoGammaModel(),1); }
    EmModel(1)->SetLowEnergyLimit(MinKinEnergy());
    EmModel(1)->SetHighEnergyLimit(MaxKinEnergy());
    AddEmModel(1, EmModel(1));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void G4eplusAnnihilationHi::PrintInfo()
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VParticleChange* G4eplusAnnihilationHi::AtRestDoIt(const G4Track& aTrack,
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
  
  fParticleChange.ProposeWeight(aTrack.GetWeight());
  // add gammas
  fParticleChange.SetNumberOfSecondaries(2);
  G4DynamicParticle* dp = 
    new G4DynamicParticle(theGamma, dir, electron_mass_c2);
  dp->SetPolarization(pol.x(),pol.y(),pol.z()); 
  fParticleChange.AddSecondary(dp);

  dp = new G4DynamicParticle(theGamma,-dir, electron_mass_c2);
  dp->SetPolarization(-pol.x(),-pol.y(),-pol.z()); 
  fParticleChange.AddSecondary(dp);

  // Kill the incident positron
  //
  fParticleChange.ProposeTrackStatus(fStopAndKill);
  return &fParticleChange;
}
