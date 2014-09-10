/*
 *Dark Photon Calorimeter Hit
 *
 *!!!History 
 *   CJC 6.17.14 created
 *
 */

#ifndef OmniHit_h
#define OmniHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"
#include "G4LogicalVolume.hh"

//Calorimeter hit class

class OmniHit : public G4VHit
{
public:
  OmniHit();
  OmniHit(const OmniHit&);
  OmniHit(G4int z);
  virtual ~OmniHit();

  //operators
  const OmniHit& operator =(const OmniHit&);
  G4int operator==(const OmniHit&) const;

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
  void SetChamber(G4int num)
  {fCham = num;};
  void SetLogV(G4LogicalVolume* vol)
  {fLogV = vol;}

  //Get methods
  G4int GetTrack()
 {return fTrackID;};
  G4double GetTotalEnergy()
  {return fTotalEnergy;};
  G4ThreeVector GetPos()
  {return fPos;};
  G4ThreeVector GetMomentum()
  {return fMomentum;};
  G4int GetChamber()
  {return fCham;};
  const G4LogicalVolume* GetLogV() const
  {return fLogV;}


private:

  G4int fTrackID;
  G4double fTotalEnergy;
  G4ThreeVector fPos;
  G4ThreeVector fMomentum;
  G4int fCham;
  const G4LogicalVolume* fLogV;
};

//Allow for a hits collection of OmniHits
typedef G4THitsCollection<OmniHit> OmniHitsCollection;

extern G4ThreadLocal G4Allocator<OmniHit>* OmniHitAllocator;


//Easy source of Segmentation Faults
inline void* OmniHit::operator new(size_t)
{
  if(!OmniHitAllocator)
    OmniHitAllocator = new G4Allocator<OmniHit>; 
  return (void *) OmniHitAllocator->MallocSingle();
}

inline void OmniHit::operator delete(void *hit)
{
  OmniHitAllocator->FreeSingle((OmniHit*) hit);
}

#endif

