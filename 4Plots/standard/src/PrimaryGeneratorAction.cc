/*
 * Dark Photon Primary Generator Action
 * !!!History
 *    CJC 6.14.15 created
 *
 * file: Primary Generator Action
 *
 * Description
 * This class sets up the experiment, including the world volume and the 
 * particle beam.
 * Methods to update:
 * PrimaryGeneratorAction
 * {adjust energy settings of the gun and particles}
 */

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Event.hh"
#include "G4SingleParticleSource.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSAngDistribution.hh"

#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{
  /*
  G4int nofParticles = 1;
  fParticleGun = new G4ParticleGun(nofParticles);

  // default particle kinematic

  G4ParticleDefinition* particleDefinition 
    //!!!    
    //Positron beam
    = G4ParticleTable::GetParticleTable()->FindParticle("e+");

  //!!!!
  // 5 GeV beam straight down the Z axis
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  fParticleGun->SetParticleEnergy(5.0*GeV);
  */
  fParticleSource = new G4GeneralParticleSource();

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


}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleSource;
}
  // This function is called at the begining of event
  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.
void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4double worldZHalfLength = 0;
  G4LogicalVolume* worldLV
    = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
  G4Box* worldBox = NULL;
  if ( worldLV ) worldBox = dynamic_cast<G4Box*>(worldLV->GetSolid());
  if ( worldBox ) worldZHalfLength = worldBox->GetZHalfLength();
  else  {
    G4cerr << "World volume of box not found." << G4endl;
    G4cerr << "Perhaps you have changed geometry." << G4endl;
    G4cerr << "The gun will be place in the center." << G4endl;
  }

  fParticleSource->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));

  fParticleSource->GeneratePrimaryVertex(anEvent);
 
}
