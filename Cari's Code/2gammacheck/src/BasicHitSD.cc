/*
 *Dark Photon Calorimeter SD
 *
 *!!!History
 *   CJC 6.17.14 created
 *   CJC 6.23.14 updated for multiple crystal calorimeter
 */

#include "BasicHitSD.hh"
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

BasicHitSD::BasicHitSD(const G4String& name,
			   const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), 
    fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
  //!!!
}


BasicHitSD::~BasicHitSD()
{
}

void BasicHitSD::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection

fHitsCollection = 
  new BasicHitsCollection(SensitiveDetectorName, collectionName[0]);

//Add collection in hit collection of the event

G4int hceID = 
  G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
 hce -> AddHitsCollection( hceID, fHitsCollection);

}

G4bool BasicHitSD::ProcessHits(G4Step* aStep, 
				  G4TouchableHistory* )
{
 
  //Only looks for gammas
  G4int PDGID = aStep -> GetTrack()->GetDefinition()->GetPDGEncoding();



  if (PDGID==22)
    {
      BasicHit* newHit = new BasicHit();
      //enters fields in Calor Hit class
      newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 

      fHitsCollection->insert(newHit);
    }

  return true;
}

void BasicHitSD::EndOfEvent(G4HCofThisEvent*)
{
}




