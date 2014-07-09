/*
 *Dark Photon Calorimeter SD
 *
 *!!!History
 *   CJC 6.17.14 created
 *   CJC 6.23.14 updated for multiple crystal calorimeter
 */

#include "TestSD.hh"

#include "G4HCofThisEvent.hh"
#include "G4ParticleDefinition.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TouchableHistory.hh"

TestSD::TestSD(const G4String& name,
			   const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), 
    fHitsCollection(NULL),
    fHCID(-1)
{
  collectionName.insert(hitsCollectionName);
}


TestSD::~TestSD()
{
}

void TestSD::Initialize(G4HCofThisEvent* hce)
{
  //Create hits collection

fHitsCollection = 
  new TestHitsCollection(SensitiveDetectorName, collectionName[0]);

//Add collection in hit collection of the event

G4int hceID = 
  G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); 
 hce -> AddHitsCollection( hceID, fHitsCollection);

 for(G4int i=0; i<1100; i++)
   {
     TestHit* hit = new TestHit(i);
     fHitsCollection->insert(hit); 
   }

}

G4bool TestSD::ProcessHits(G4Step* step, 
				  G4TouchableHistory* )
{
  G4double edep = step->GetTotalEnergyDeposit();
  if (edep==0.) return true;

  G4TouchableHistory* touchable 
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* physical = touchable->GetVolume();
  G4int copyNo = physical->GetCopyNo();
  G4int regionNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1);
 

  TestHit* hit = (*fHitsCollection)[copyNo]; 

  if(!(hit->GetLogV()))
    {
      hit->SetLogV(physical->GetLogicalVolume());
    }

  hit->AddEdep(edep);
  hit->SetPos(step->GetPostStepPoint()->GetPosition());
  hit->SetCellID(copyNo);
  hit->SetRegion(regionNo);
  if (regionNo==1) {
    hit->SetRow((copyNo/36)+7); 
    hit->SetCol((copyNo%36)-18); }
  if (regionNo==2) {
    hit->SetRow((copyNo/22)-7);
    G4int colC = copyNo%22; 
    if (colC<11) {hit->SetCol(colC-18);}
    else {hit->SetCol(colC+14);}
  }
  if (regionNo==3) {
    hit->SetRow((copyNo/36)-18);
    hit->SetCol((copyNo%36)-18);
  }


  return true;
}





