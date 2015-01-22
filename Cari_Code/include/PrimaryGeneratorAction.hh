/*
 * Dark Photon Generator Action
 * !!!History
 *    CJC 6.15.14 created
 */

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

//!!!
//May have to change depending on nature of final beam
class G4ParticleGun;
class G4Event; 

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* );

  G4ParticleGun* GetParticleGun() {return fParticleGun1;}

  //Set methods
  void SetRandomFlag(G4bool );

private :
  G4ParticleGun* fParticleGun1; //G4 particle gun
  G4ParticleGun* fParticleGun2;
  G4ParticleGun* fParticleGun3;
  G4ParticleGun* fParticleGun4;

};


#endif
