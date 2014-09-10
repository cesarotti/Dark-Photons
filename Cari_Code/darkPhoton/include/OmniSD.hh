/*
 * Dark Photon Calorimeter Sensitive Detector
 *
 * !!!History 
 *    CJC 6.17.14 created
 */

#ifndef OmniSD_h
#define OmniSD_h 1

#include "G4VSensitiveDetector.hh"

#include "OmniHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

class OmniSD : public G4VSensitiveDetector
{
public:
  OmniSD(const G4String& name, 
		const G4String& hitsCollectionName);
  virtual ~OmniSD();

  //methods from base class
  virtual void Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

private:
  OmniHitsCollection* fHitsCollection; 
  G4double fOHID;
};

#endif
