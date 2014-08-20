#ifndef DarkPhysicsList_h
#define DarkPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

class DarkPhysicsList: public G4VModularPhysicsList
{
public:
  DarkPhysicsList();
  virtual ~DarkPhysicsList();
  /*
  void ConstructParticle();
  void ConstructProcess();
  */
  virtual void SetCuts();

  /*
private:
  void ConstructEM();
  */
  
};

#endif

