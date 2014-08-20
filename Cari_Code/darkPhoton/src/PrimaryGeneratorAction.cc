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
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4SPSPosDistribution.hh"
#include "G4SPSAngDistribution.hh"

#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction()
{ 
  /*
  G4double energySig = 250.*MeV;
  G4double energy  = G4RandGauss::shoot(500.*MeV, energySig);
  */

  G4double energy = 5.*GeV;
  G4int nofParticles = 1;
  fParticleGun1 = new G4ParticleGun(nofParticles);

  G4ParticleDefinition* particleDefinition 
    = G4ParticleTable::GetParticleTable()->FindParticle("e+");
  fParticleGun1->SetParticleDefinition(particleDefinition);
  fParticleGun1->SetParticleMomentumDirection(G4ThreeVector(0.,0., 1.));
  fParticleGun1->SetParticleEnergy(energy);
  
  /*
  
  fParticleGun2 = new G4ParticleGun(nofParticles);
  //energy  = G4RandGauss::shoot(500.*MeV, energySig);
  fParticleGun2->SetParticleDefinition(particleDefinition);
  fParticleGun2->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  fParticleGun2->SetParticleEnergy(energy);

  fParticleGun3 = new G4ParticleGun(nofParticles);
  //energy  = G4RandGauss::shoot(500.*MeV, energySig);
  fParticleGun3->SetParticleDefinition(particleDefinition);
  fParticleGun3->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  fParticleGun3->SetParticleEnergy(energy);

  
  fParticleGun4 = new G4ParticleGun(nofParticles);
  energy  = G4RandGauss::shoot(500.*MeV, energySig);
  fParticleGun4->SetParticleDefinition(particleDefinition);
  fParticleGun4->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
  fParticleGun4->SetParticleEnergy(energy);
  */

 
  /*
  
  fParticleSource= new G4GeneralParticleSource();

G4ParticleDefinition* particleDefinition = 
  G4ParticleTable::GetParticleTable()->FindParticle("e+");

 fParticleSource->GetCurrentSource()->SetParticleDefinition(particleDefition);
 fParticleSource->GetCurrentSource()->GetPosDist()
   ->SetCentreCoords(G4ThreeVector(0,0, -5.*m));
  fParticleSource->GetCurrentSource()->GetPosDist()
    ->SetPosRot1(G4ThreeVector(1,0,0));
  fParticleSource->GetCurrentSource()->GetPosDist()
    ->SetPosRot2(G4ThreeVector(0,1,0));

  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisType("Beam");
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisShape("Square");
  fParticleSource->GetCurrentSource()->GetPosDist()->SetHalfX(17*5.0*cm);
  fParticleSource->GetCurrentSource()->GetPosDist()->SetHalfY(17*5.0*cm);

  fParticleSource->GetCurrentSource()->GetAngDist()->SetAngDistType("beam1d");
  fParticleSource->GetCurrentSource()->GetAngDist()
    ->DefineAngRefAxes("angref1", G4ThreeVector(-1.,0.,0.)); 
  fParticleSource->GetCurrentSource()->GetAngDist()
    ->DefineAngeRefAxes("angref2", G4ThreeVector(0.,1.,0.));

  */



}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun1;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  G4cout << "Generating Primaries" << G4endl;
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

  /*
  G4double sigma = 17*5.0*cm;
  G4double sigX = G4RandGauss::shoot(0., sigma);
  G4double sigY = G4RandGauss::shoot(0., sigma);

  fParticleGun1->SetParticlePosition(G4ThreeVector(sigX, sigY, worldZHalfLength));
*/
  fParticleGun1->SetParticlePosition(G4ThreeVector(0., 0., -worldZHalfLength));
  fParticleGun1->GeneratePrimaryVertex(anEvent);

  /*
  sigX = G4RandGauss::shoot(0., sigma);
  sigY = G4RandGauss::shoot(0., sigma);
  fParticleGun2->SetParticlePosition(G4ThreeVector(sigX, sigY, worldZHalfLength));
  
 
  
  fParticleGun2->SetParticlePosition(G4ThreeVector(13.49*5.*cm, 0., worldZHalfLength));
   fParticleGun2->GeneratePrimaryVertex(anEvent);
  
   
   
  sigX = G4RandGauss::shoot(0., sigma);
  sigY = G4RandGauss::shoot(0., sigma);
  fParticleGun3->SetParticlePosition(G4ThreeVector(sigX, sigY, worldZHalfLength));
   
   
   fParticleGun3->SetParticlePosition(G4ThreeVector(-13.49*5.0*cm, .49*5.0*cm, worldZHalfLength));
   
   fParticleGun3->GeneratePrimaryVertex(anEvent);
   

  
  sigX = G4RandGauss::shoot(0., sigma);
  sigY = G4RandGauss::shoot(0., sigma);
  fParticleGun4->SetParticlePosition(G4ThreeVector(sigX, sigY, worldZHalfLength));

   fParticleGun4->GeneratePrimaryVertex(anEvent);
   */

}
