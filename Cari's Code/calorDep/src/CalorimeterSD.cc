/*
 *Dark Photon Calorimeter SD
 *
 *!!!History
 *   CJC 6.17.14 created
 *   CJC 6.23.14 updated for multiple crystal calorimeter
 */
#include "CalorimeterSD.hh"
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

CalorimeterSD::CalorimeterSD(const G4String& name,
			   const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), 
    fHitsCollection(NULL),
    thresholdEnergy(0.) // minimal energy to count as a hit
{
  collectionName.insert(hitsCollectionName);
  //!!!
  thresholdEnergy = 1.0*MeV;
}


CalorimeterSD::~CalorimeterSD()
{
}

void CalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection

fHitsCollection = 
  new CalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]);

//Add collection in hit collection of the event

G4int hceID = 
  G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
 hce -> AddHitsCollection( hceID, fHitsCollection);

}

G4bool CalorimeterSD::ProcessHits(G4Step* aStep, 
				  G4TouchableHistory* )
{
 
  //Only looks for gammas
  G4int PDGID = aStep -> GetTrack()->GetDefinition()->GetPDGEncoding();


  //!!!
  //Defining energy threshold

  if(aStep->GetTrack()->GetTotalEnergy() < thresholdEnergy) return false;

  //checking for only gammas

  if (PDGID==22)
    {
      CalorHit* newHit = new CalorHit();
      //enters fields in Calor Hit class
      newHit->SetTrackID (aStep->GetTrack()->GetTrackID());
      newHit->SetTotalEnergy(aStep->GetTrack()->GetTotalEnergy()); 
      newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());
      G4int check = aStep->GetPreStepPoint()->GetTouchableHandle()
	->GetCopyNumber(1);
      if (check<1)
	{newHit->SetColumn(-1);}
      else 
{newHit->SetColumn(aStep->GetPreStepPoint()
		   ->GetTouchableHandle()->GetReplicaNumber());}

      newHit->SetRow(check);


      newHit->SetMomentum(aStep->GetTrack()->GetMomentum());
     
      fHitsCollection->insert(newHit);
    }

  return true;
}

void CalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{
	G4int nofHits = fHitsCollection->entries();
	for (G4int i =0; i<nofHits; i++) (*fHitsCollection)[i]->Print();
}




