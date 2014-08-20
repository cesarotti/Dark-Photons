
#ifndef PositronPhysicsList1_h
#define PositronPhysicsList1_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class PositronPhysicsList1 : public G4VUserPhysicsList
{
public:
  PositronPhysicsList1();
  virtual ~PositronPhysicsList1();
 
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();

private:
  
  void ConstructEM();
  void ConstructBremsstrahlung();
  void ConstructPositronNuclear();
  void ConstructElectronNuclear();

};

#endif
