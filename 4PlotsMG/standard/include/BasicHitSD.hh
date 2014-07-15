/*
 * Dark Photon Calorimeter Sensitive Detector
 *
 * !!!History 
 *    CJC 6.17.14 created
 */

#ifndef BasicHitSD_h
#define BasicHitSD_h 1

#include "G4VSensitiveDetector.hh"

#include "BasicHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class BasicHitSD : public G4VSensitiveDetector
{
public:
  BasicHitSD(const G4String& name, 
	     const G4String& hitsCollectionName);

  virtual ~BasicHitSD();

  //methods from base class
  virtual void Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

  void SetSmearing(G4bool smear)
  {fSmearing = smear;};

private:
  BasicHitsCollection* fHitsCollection; 
  G4bool fSmearing; 
};

#endif
