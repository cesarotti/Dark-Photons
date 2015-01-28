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

<<<<<<< HEAD
 for(G4int i=0; i<15; i++) //15 sectors
=======
 for(G4int i=0; i<1; i++) //15 sectors
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
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
<<<<<<< HEAD

  OmniHit* hit = (*fHitsCollection)[copyNo]; 

  if(!(hit->GetLogV()))
=======
  G4double energyTots = step->GetTrack()->GetTotalEnergy();

  if ( step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary && energyTots > 50*MeV ){
  OmniHit* hit = (*fHitsCollection)[copyNo]; 

  if(!(hit->GetLogV())&&!(hit->GetHit()))
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
    {
      hit->SetLogV(physical->GetLogicalVolume());
    }

  hit->SetPos(step->GetPostStepPoint()->GetPosition());
  hit->SetMomentum(step->GetTrack()->GetMomentum());
<<<<<<< HEAD
  hit->SetTotalEnergy(step->GetTrack()->GetTotalEnergy()); 
  hit->SetChamber(copyNo);
=======
  hit->SetTotalEnergy(energyTots); 
  hit->SetChamber(copyNo);
  hit->SetHit(true);
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
  
  /*
  OmniHit* hit = new OmniHit();
  
  hit->SetPos(step->GetPostStepPoint()->GetPosition());;
  hit->SetMomentum(step->GetTrack()->GetMomentum());
  hit->SetTotalEnergy(step->GetTrack()->GetTotalEnergy());

  fHitsCollection->insert(hit);
  */
<<<<<<< HEAD

=======
  }
>>>>>>> 911b5ec345c81b054214c521136984e9d6306827
  return true; 
}

void OmniSD::EndOfEvent(G4HCofThisEvent*)
{
}



