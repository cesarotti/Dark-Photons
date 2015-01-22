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
  G4cout << "Primaries started" << G4endl;
  fParticleSource = new G4GeneralParticleSource();

  // default particle kinematic

  G4ParticleDefinition* particleDefinition 
    //!!!    
    //Positron beam
    = G4ParticleTable::GetParticleTable()->FindParticle("gamma");

  fParticleSource->GetCurrentSource()->SetParticleDefinition(particleDefinition);

  fParticleSource->GetCurrentSource()->GetPosDist()->SetCentreCoords(G4ThreeVector(0,0,-10.5*m));
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosRot1(G4ThreeVector(1,0,0));
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosRot2(G4ThreeVector(0,1,0));

  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisType("Beam");
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosDisShape("Circle");
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosRot1(G4ThreeVector(1.0,0.,0.));
  fParticleSource->GetCurrentSource()->GetPosDist()->SetPosRot2(G4ThreeVector(0.,-1.0,0.));

  fParticleSource->GetCurrentSource()->GetPosDist()->SetRadius(1e-6*mm);
  fParticleSource->GetCurrentSource()->GetPosDist()->SetBeamSigmaInR(3*mm);
  
  fParticleSource->GetCurrentSource()->GetAngDist()->SetAngDistType("user");
  fParticleSource->GetCurrentSource()->GetAngDist()->DefineAngRefAxes("angref1", G4ThreeVector(1.0,0.,0.));
  fParticleSource->GetCurrentSource()->GetAngDist()->DefineAngRefAxes("angref2", G4ThreeVector(0.,1.0,0.));

  G4double thetamin=175*deg;
  G4double thetamax=178*deg;
  double beta = -3.01;
  int nbin=100;
  fParticleSource->GetCurrentSource()->GetAngDist()->UserDefAngTheta(G4ThreeVector(thetamin,0,0));
  for (int i=1; i<=nbin; i++){
    G4double upperbound = thetamin + (thetamax - thetamin) / nbin * i;
    G4double mid = thetamin + (thetamax - thetamin) / nbin * i / 2;
    fParticleSource->GetCurrentSource()->GetAngDist()->UserDefAngTheta(G4ThreeVector(upperbound,pow((180*deg-mid)/rad,beta),0));
  }

  //fParticleSource->GetAngDist()->SetMaxTheta(1e-6*radian);
  //fParticleSource->GetCurrentSource()->GetAngDist()->SetBeamSigmaInAngR(2e-4*radian);
   //!!!
   //Energy Set to 5 GeV
  
  //fParticleSource->GetAngDist()->SetAngDistType("planar");
  //fParticleSource->GetAngDist()->SetParticleMomentumDirection(G4ThreeVector(0,0,1));

  //fParticleSource->GetEneDist()->SetMonoEnergy(5*GeV);
  
  fParticleSource->GetCurrentSource()->GetEneDist()->SetEnergyDisType("Pow");
  G4double min=0.1*MeV;
  G4double max=2000*MeV;
  G4double mass=particleDefinition->GetPDGMass();
  fParticleSource->GetCurrentSource()->GetEneDist()->SetEmax(max);
  fParticleSource->GetCurrentSource()->GetEneDist()->SetEmin(min);
  fParticleSource->GetCurrentSource()->GetEneDist()->SetAlpha(-1.03);

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

  fParticleSource->SetParticlePosition(G4ThreeVector(0., 0., -5*m));

  fParticleSource->GeneratePrimaryVertex(anEvent);
 
}
