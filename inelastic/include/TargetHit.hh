/*
 *Dark Photon Target Hit
 *
 * History:
 *     BYS 6.26.14 Created
 *
 */

#ifndef TargetHit_h
#define TargetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

//Targetimeter hit class

class TargetHit : public G4VHit
{
public:
  TargetHit();
  virtual ~TargetHit();

  //operators
  const TargetHit& operator =(const TargetHit&);
  G4int operator==(const TargetHit&) const;

  inline void* operator new(size_t);
  inline void operator delete(void*);

  //methods from base class
  virtual void Draw();
  virtual void Print();

  //Set methods
  void SetTrackID (G4int track)
  { fTrackID = track;};
  void SetTotalEnergy(G4double energy)
  {fTotalEnergy = energy;};
  void SetPos(G4ThreeVector pos)
  {fPos = pos;};
  void SetMomentum(G4ThreeVector pv)
  {fMomentum = pv;};
  void SetPDGID(G4int id)
  {fPDGID = id;};

  //Get methods
  G4int GetTrack()
  {return fTrackID;};
  G4double GetTotalEnergy()
  {return fTotalEnergy;};
  G4ThreeVector GetPos()
  {return fPos;};
  G4ThreeVector GetMomentum()
  {return fMomentum;};
  G4int GetPDGID()
  {return fPDGID;};

private:

  G4int fTrackID;
  G4double fTotalEnergy;
  G4ThreeVector fPos;
  G4ThreeVector fMomentum;
  G4int fPDGID;
};

//Allow for a hits collection of TargetHits
typedef G4THitsCollection<TargetHit> TargetHitsCollection;

extern G4ThreadLocal G4Allocator<TargetHit>* TargetHitAllocator;


//Easy source of Segmentation Faults
inline void* TargetHit::operator new(size_t)
{
  if(!TargetHitAllocator)
    TargetHitAllocator = new G4Allocator<TargetHit>; 
  return (void *) TargetHitAllocator->MallocSingle();
}

inline void TargetHit::operator delete(void *hit)
{
  TargetHitAllocator->FreeSingle((TargetHit*) hit);
}

#endif

