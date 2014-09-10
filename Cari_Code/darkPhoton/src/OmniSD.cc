/*
 *Dark Photon Calorimeter SD
 *
 *!!!History
 *   CJC 6.17.14 created
 *   CJC 6.23.14 updated for multiple crystal calorimeter
 */

#include "OmniSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

OmniSD::OmniSD(const G4String& name,
			   const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), 
    fHitsCollection(NULL), 
    fOHID(-1)
{
  collectionName.insert(hitsCollectionName);

}


OmniSD::~OmniSD()
{
}

void OmniSD::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection
  
fHitsCollection = 
  new OmniHitsCollection(SensitiveDetectorName, collectionName[0]);

//Add collection in hit collection of the event

G4int hceID = 
  G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
 hce -> AddHitsCollection( hceID, fHitsCollection);

 for(G4int i=0; i<15; i++) //15 sectors
   {
    OmniHit* hit = new OmniHit(i); 
    fHitsCollection->insert(hit);
   } 
  

  /*
  fHitsCollection 
    = new OmniHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce

  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 
  */
}

G4bool OmniSD::ProcessHits(G4Step* step, 
				  G4TouchableHistory* )
{
  
  G4TouchableHistory* touchable 
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* physical = touchable->GetVolume();
  G4int copyNo = physical->GetCopyNo();

  OmniHit* hit = (*fHitsCollection)[copyNo]; 

  if(!(hit->GetLogV()))
    {
      hit->SetLogV(physical->GetLogicalVolume());
    }

  hit->SetPos(step->GetPostStepPoint()->GetPosition());
  hit->SetMomentum(step->GetTrack()->GetMomentum());
  hit->SetTotalEnergy(step->GetTrack()->GetTotalEnergy()); 
  hit->SetChamber(copyNo);
  
  /*
  OmniHit* hit = new OmniHit();
  
  hit->SetPos(step->GetPostStepPoint()->GetPosition());;
  hit->SetMomentum(step->GetTrack()->GetMomentum());
  hit->SetTotalEnergy(step->GetTrack()->GetTotalEnergy());

  fHitsCollection->insert(hit);
  */

  return true; 
}

void OmniSD::EndOfEvent(G4HCofThisEvent*)
{
}




