#ifndef EmStandardPhysics_h
#define EmStandardPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class EmStandardPhysics : public G4VPhysicsConstructor
{
    public:
      EmStandardPhysics(G4int ver = 0);
    
      // obsolete
      EmStandardPhysics(G4int ver, const G4String& name);
    
  virtual ~EmStandardPhysics();
  
  virtual void ConstructParticle();
  virtual void ConstructProcess();
    
    private:
      G4int  verbose;
    };
 #endif
