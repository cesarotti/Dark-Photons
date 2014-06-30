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
class ExN04PrimaryGeneratorAction;
class PrimaryGeneratorMessenger;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* );

  G4ParticleGun* GetParticleGun() {return fParticleGun;}

 void SelectAction(G4int i) { fSelectedAction = i; };    
 G4int GetSelectedAction()  { return fSelectedAction; };    
 ExN04PrimaryGeneratorAction* GetAction1() { return fAction1; };

  //Set methods
  void SetRandomFlag(G4bool );

private :
  G4ParticleGun* fParticleGun; //G4 particle gun
  ExN04PrimaryGeneratorAction* fAction1;
  PrimaryGeneratorMessenger* fGunMessenger;
  G4int                    fSelectedAction;
};


#endif
