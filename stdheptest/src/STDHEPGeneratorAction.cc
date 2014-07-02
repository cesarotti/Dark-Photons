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

#include "Randomize.hh"




#include "STDHEPGeneratorAction.hh"

#include "G4Event.hh"
#include "G4HEPEvtInterface.hh"

ExN04PrimaryGeneratorAction::ExN04PrimaryGeneratorAction()
{
  G4cout << "RUNNING STDHEP OPEN" << G4endl;
  HEPEvt = new G4HEPEvtInterface("/media/sf_darkphotons/stdheptest/inputSTDHEP/ba.dat");
}

ExN04PrimaryGeneratorAction::~ExN04PrimaryGeneratorAction()
{
  delete HEPEvt;
}

void ExN04PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  G4cout << "RUNNING STDHEP" << G4endl;
  HEPEvt->SetParticlePosition(G4ThreeVector(0.*cm,0.*cm,0.*cm));
  HEPEvt->GeneratePrimaryVertex(anEvent);
}