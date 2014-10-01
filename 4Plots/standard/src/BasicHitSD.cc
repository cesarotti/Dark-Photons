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
#include "Randomize.hh"

/*
 * boolean variable fSmearing determines if energy and position smearing
 * are considered
 */

BasicHitSD::BasicHitSD(const G4String& name,
		       const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), 
    fHitsCollection(NULL),
    fSmearing(0)
{
  collectionName.insert(hitsCollectionName);
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
 
  G4double lowThreshold = 10*MeV; 
  G4double enTotal = aStep->GetTrack()->GetTotalEnergy();
  if (enTotal < lowThreshold) {return true;}

  if ( aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary ){
  BasicHit* hit = new BasicHit();

  G4int PDGID = aStep -> GetTrack()->GetDefinition()->GetPDGEncoding();
  hit->SetPDGID(PDGID); //Can use to discriminate types of particles counted
 
  /*
   * Figures from CLEO_CsI_Summary.pdf (Jim Alexander 2014)
   */
  if (fSmearing)
 {
   enTotal *= GeV/(1000.*MeV);
   hit->SetTotalEnergy(G4RandGauss::shoot(enTotal,.03*pow(enTotal,0.5)));

   enTotal *= 1000.*MeV/GeV;

   G4double posSig = (90.*mm)*pow(enTotal, -0.5);

    G4ThreeVector position = aStep->GetTrack()->GetPosition();
    G4double sigX = G4RandGauss::shoot(position.getX(), posSig);
    G4double sigY = G4RandGauss::shoot(position.getY(), posSig);
    hit->SetPosition(G4ThreeVector(sigX, sigY, position.getZ()));

 }

    else {
      hit->SetPosition(aStep->GetTrack()->GetPosition());
      hit->SetTotalEnergy(enTotal);  }

  fHitsCollection->insert(hit);
  }

  return true;
}

void BasicHitSD::EndOfEvent(G4HCofThisEvent*)
{
}




