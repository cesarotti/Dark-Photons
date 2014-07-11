/*
 *Dark Photon Calorimeter Hit
 *
 *!!!History 
 *   CJC 6.17.14 created
 *
 */

#ifndef BasicHit_h
#define BasicHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

//Calorimeter hit class

class BasicHit : public G4VHit
{
public:
  BasicHit();
  BasicHit(const BasicHit&);
  virtual ~BasicHit();

  //operators
  const BasicHit& operator =(const BasicHit&);
  G4int operator==(const BasicHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  //methods from base class
  virtual void Draw();
  virtual void Print();

  //Set methods

  void SetTotalEnergy(G4double energy)
  {fTotalEnergy = energy;};


  //Get methods

  G4double GetTotalEnergy()
  {return fTotalEnergy;};


private:

  G4double fTotalEnergy;
};

//Allow for a hits collection of BasicHits
typedef G4THitsCollection<BasicHit> BasicHitsCollection; 

extern G4ThreadLocal G4Allocator<BasicHit>* BasicHitAllocator;


//Easy source of Segmentation Faults
inline void* BasicHit::operator new(size_t)
{
  if(!BasicHitAllocator)
    BasicHitAllocator = new G4Allocator<BasicHit>; 
  return (void *) BasicHitAllocator->MallocSingle();
}

inline void BasicHit::operator delete(void *hit)
{
  BasicHitAllocator->FreeSingle((BasicHit*) hit);
}

#endif
