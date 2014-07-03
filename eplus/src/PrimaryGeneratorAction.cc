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
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSAngDistribution.hh"

#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{
  G4cout << "Primaries started" << G4endl;
  fParticleSource = new G4SingleParticleSource();

  // default particle kinematic

  G4ParticleDefinition* particleDefinition 
    //!!!    
    //Positron beam
    = G4ParticleTable::GetParticleTable()->FindParticle("e+");

  fParticleSource->SetParticleDefinition(particleDefinition);

  fParticleSource->GetPosDist()->SetCentreCoords(G4ThreeVector(0,0,-8*m));
  fParticleSource->GetPosDist()->SetPosRot1(G4ThreeVector(1,0,0));
  fParticleSource->GetPosDist()->SetPosRot2(G4ThreeVector(0,1,0));

  fParticleSource->GetPosDist()->SetPosDisType("Beam");
  fParticleSource->GetPosDist()->SetPosDisShape("Circle");

  fParticleSource->GetPosDist()->SetRadius(1e-6*mm);
  fParticleSource->GetPosDist()->SetBeamSigmaInR(1*mm);
   
  fParticleSource->GetAngDist()->SetAngDistType("beam1d");
  fParticleSource->GetAngDist()->DefineAngRefAxes("angref1", G4ThreeVector(-1,0,0));
  fParticleSource->GetAngDist()->DefineAngRefAxes("angref2", G4ThreeVector(0,1,0));
  //fParticleSource->GetAngDist()->SetMaxTheta(1e-6*radian);
  fParticleSource->GetAngDist()->SetBeamSigmaInAngR(1e-3*radian);
   //!!!
   //Energy Set to 5 GeV
  
  //fParticleSource->GetAngDist()->SetAngDistType("planar");
  //fParticleSource->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0,0,1));

  //fParticleSource->GetEneDist()->SetMonoEnergy(5*GeV);
  
  fParticleSource->GetEneDist()->SetEnergyDisType("Lin");
  G4double max=5*GeV;
  G4double range=50*MeV;
  fParticleSource->GetEneDist()->SetEmax(max);
  fParticleSource->GetEneDist()->SetEmin(max-range);
  fParticleSource->GetEneDist()->SetGradient(0);
  fParticleSource->GetEneDist()->SetInterCept(1);


  G4cout << "Primaries finished" << G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  //G4cout << "Generating Primaries" << G4endl;
  // This function is called at the begining of event

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get world volume
  // from G4LogicalVolumeStore.

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
